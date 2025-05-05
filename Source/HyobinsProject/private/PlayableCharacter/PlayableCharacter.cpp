// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacter/PlayableCharacter.h"
#include "MotionWarpingComponent.h"
#include <GameFramework/SpringArmComponent.h.>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "Utility/Utility.h"
#include "SubSystems/UIManager.h"

const int32 APlayableCharacter::DirectionIndex[3][3] =
	{{5,4,3},
	{6,0,2},
	{7,0,1}
	};

APlayableCharacter::APlayableCharacter() :
	m_CurInputHorizontal(0),
	m_CurInputVertical(0),
	m_MoveDeltaSecondsOffset(20000.0f),
	m_RotationDeltaSecondsOffset(50.0f),
	m_OnHitCameraShake(nullptr),
	m_AttackCameraShake(nullptr)
{
	initAssets();
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	uiManager->CreateMainPlayerStatusBar(this->m_StatComponent, this);
	OnChangeInputMappingContext.AddUObject(uiManager, &UUIManager::ChangeSkillList);

	initInputConfigs();
}

void APlayableCharacter::RotateActorToKeyInputDirection() // WSAD 키입력방향으로 액터회전.
{
	FRotator actorRotation = GetActorRotation();
	const double degree = Utility::ConvertToDegree(m_CurInputVertical, m_CurInputHorizontal);
	actorRotation.Yaw = GetControlRotation().Yaw + degree;
	
	SetActorRotation(actorRotation);
}

void APlayableCharacter::RotateActorToControllerYaw() // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.
{
	FRotator actorRotation = GetActorRotation();
	actorRotation.Yaw =  GetControlRotation().Yaw;
	
	SetActorRotation(actorRotation);
}

void APlayableCharacter::AddInputMappingContext(const FName& inputMappingContextName)
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	UInputMappingContext* inputMappingContext = m_InputMappingConfigs[inputMappingContextName].inputMappingContext;
	
	if (!subSystem->HasMappingContext(inputMappingContext))
	{
		const int32 priority = m_CurActiveMappingContexts.Num();
		m_CurActiveMappingContexts.Add(inputMappingContextName, priority);
		subSystem->AddMappingContext(inputMappingContext,priority);
		
		OnChangeInputMappingContext.Broadcast();
	}
}

void APlayableCharacter::RemoveInputMappingContext(const FName& inputMappingContextName)
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	UInputMappingContext* inputMappingContext = m_InputMappingConfigs[inputMappingContextName].inputMappingContext;
	
	if (subSystem->HasMappingContext(inputMappingContext))
	{
		subSystem->RemoveMappingContext(inputMappingContext);
		m_CurActiveMappingContexts.Remove(inputMappingContextName);
		
		OnChangeInputMappingContext.Broadcast();
	}
}

FName APlayableCharacter::GetHighestPriorityInputMappingContext() const
{
	int32 highestPriority = -1;
	FName highestPriorityInputMappingContext = "";
	
	for (auto& iter : m_CurActiveMappingContexts)
	{
		if (iter.Value > highestPriority)
		{
			highestPriority = iter.Value;
			highestPriorityInputMappingContext = iter.Key;
		}
	}

	return highestPriorityInputMappingContext;
}

FVector APlayableCharacter::GetForwardVectorFromControllerYaw() const
{
	const FRotator rotator = {0.0f, GetControlRotation().Yaw, 0.0f};
	
	return rotator.Vector();
}

FVector APlayableCharacter::GetRightVectorFromControllerYaw() const
{
	const FRotator controllerYawRotator(0.0f, GetControlRotation().Yaw, 0.0f); // 컨트롤러 Yaw를 기준으로한 임시값.
	const FVector rightVector = FRotationMatrix(controllerYawRotator).GetUnitAxis(EAxis::Y);  // Right 방향
	
	return rightVector;
}

FVector APlayableCharacter::GetControllerKeyInputDirection(const int32 keyInputDirection) const
{
	// keyInputDirection == 0 ~ 7까지의 8방향. 전방 ~ 좌상.
	
	//const float controllerYaw = GetController()->GetControlRotation().Yaw + 45.0f * keyInputDirection;
	const float controllerYaw = 45.0f * keyInputDirection;
	const FRotator rotation = {0.0f, controllerYaw, 0.0f};
	
	return rotation.Vector();
}

int32 APlayableCharacter::GetLocalDirection(const FVector& otherDirectionVector) const
{
	const FVector localDirection = GetActorTransform().InverseTransformVector(otherDirectionVector);
	const float radian = FMath::Atan2(localDirection.Y, localDirection.X);

	constexpr float range = PI / 8; // 22.5도

	if (radian >= -range && radian < range) // 앞쪽
	{
		return 0;
	}

	if (radian >= range && radian < 3 * range) 
	{
		return 1;
	}
	
	if (radian >= 3 * range && radian < 5 *	range) // 오른쪽
	{
		return 2;
	}

	if (radian >= 5 * range && radian < 7 * range) 
	{
		return 3;
	}

	// if ((radian < -7 * range && radian >= -8 * range) ||
	// 	(radian >= 7 * range && radian <= 8 * range)) // 뒤쪽
	// {
	// 	return 4; 
	// }
	
	if (radian < -5 * range && radian >= -7 * range) 
	{
		return 5;
	}

	if (radian < -3 * range && radian >= -5 * range) // 왼쪽
	{
		return 6;
	}
	
	if (radian < -range && radian >= -3 * range) 
	{
		return 7;
	}
	
	return 4;
}

void APlayableCharacter::initAssets()
{
	m_MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	// SpringArm
	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	m_SpringArm->TargetArmLength = 600;

	// 스프링암의 회전 값을 컨트롤 회전 값과 동일하게 맞춰준다.
	m_SpringArm->bUsePawnControlRotation = true;
	m_SpringArm->bInheritPitch = true;
	m_SpringArm->bInheritRoll = true;
	m_SpringArm->bInheritYaw = true;

	// true로 할 경우, 카메라와 캐릭터사이에 장애물이 있을 경우, 줌 기능을 활성화 해준다.
	m_SpringArm->bDoCollisionTest = false;

	// TargetCamera
	m_TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	m_TargetCamera->SetupAttachment(m_SpringArm);

	// true로 할 경우, 컨트롤러의 회전방향으로 캐릭터를 회전시켜줌.
	bUseControllerRotationYaw = false;

	// true로 할 경우, 캐릭터가 움직이려는 방향으로 캐릭터를 자동으로 회전시켜 준다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// 컨트롤러가 "원하는" 방향으로 캐릭터를 회전한다.
	// 즉, 오른쪽+위를 누르면 "정확히" 45도 방향으로 캐릭터가 회전해서 이동하는 식이다.
	// 실제로 캐릭터의 회전 방향이 "딱딱 떨어지는" 느낌을 준다.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 회전을 부드럽게 만들어 주기 위해 RotationRate 를 조정한다. 값이 낮을수록 카메라 회전시 캐릭터가 한박자 느리게 회전.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void APlayableCharacter::initInputConfigs()
{
	for (auto& iter : m_InputMappingConfigs)
	{
		iter.Value.inputMappingContextName = iter.Key;
	}
}
