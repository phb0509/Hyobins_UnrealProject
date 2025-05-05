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

void APlayableCharacter::RotateActorToKeyInputDirection() // WSAD Ű�Է¹������� ����ȸ��.
{
	FRotator actorRotation = GetActorRotation();
	const double degree = Utility::ConvertToDegree(m_CurInputVertical, m_CurInputHorizontal);
	actorRotation.Yaw = GetControlRotation().Yaw + degree;
	
	SetActorRotation(actorRotation);
}

void APlayableCharacter::RotateActorToControllerYaw() // ������ z��ȸ������ ��Ʈ�ѷ��� z��ȸ�������� ����.
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
	const FRotator controllerYawRotator(0.0f, GetControlRotation().Yaw, 0.0f); // ��Ʈ�ѷ� Yaw�� ���������� �ӽð�.
	const FVector rightVector = FRotationMatrix(controllerYawRotator).GetUnitAxis(EAxis::Y);  // Right ����
	
	return rightVector;
}

FVector APlayableCharacter::GetControllerKeyInputDirection(const int32 keyInputDirection) const
{
	// keyInputDirection == 0 ~ 7������ 8����. ���� ~ �»�.
	
	//const float controllerYaw = GetController()->GetControlRotation().Yaw + 45.0f * keyInputDirection;
	const float controllerYaw = 45.0f * keyInputDirection;
	const FRotator rotation = {0.0f, controllerYaw, 0.0f};
	
	return rotation.Vector();
}

int32 APlayableCharacter::GetLocalDirection(const FVector& otherDirectionVector) const
{
	const FVector localDirection = GetActorTransform().InverseTransformVector(otherDirectionVector);
	const float radian = FMath::Atan2(localDirection.Y, localDirection.X);

	constexpr float range = PI / 8; // 22.5��

	if (radian >= -range && radian < range) // ����
	{
		return 0;
	}

	if (radian >= range && radian < 3 * range) 
	{
		return 1;
	}
	
	if (radian >= 3 * range && radian < 5 *	range) // ������
	{
		return 2;
	}

	if (radian >= 5 * range && radian < 7 * range) 
	{
		return 3;
	}

	// if ((radian < -7 * range && radian >= -8 * range) ||
	// 	(radian >= 7 * range && radian <= 8 * range)) // ����
	// {
	// 	return 4; 
	// }
	
	if (radian < -5 * range && radian >= -7 * range) 
	{
		return 5;
	}

	if (radian < -3 * range && radian >= -5 * range) // ����
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

	// ���������� ȸ�� ���� ��Ʈ�� ȸ�� ���� �����ϰ� �����ش�.
	m_SpringArm->bUsePawnControlRotation = true;
	m_SpringArm->bInheritPitch = true;
	m_SpringArm->bInheritRoll = true;
	m_SpringArm->bInheritYaw = true;

	// true�� �� ���, ī�޶�� ĳ���ͻ��̿� ��ֹ��� ���� ���, �� ����� Ȱ��ȭ ���ش�.
	m_SpringArm->bDoCollisionTest = false;

	// TargetCamera
	m_TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	m_TargetCamera->SetupAttachment(m_SpringArm);

	// true�� �� ���, ��Ʈ�ѷ��� ȸ���������� ĳ���͸� ȸ��������.
	bUseControllerRotationYaw = false;

	// true�� �� ���, ĳ���Ͱ� �����̷��� �������� ĳ���͸� �ڵ����� ȸ������ �ش�.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// ��Ʈ�ѷ��� "���ϴ�" �������� ĳ���͸� ȸ���Ѵ�.
	// ��, ������+���� ������ "��Ȯ��" 45�� �������� ĳ���Ͱ� ȸ���ؼ� �̵��ϴ� ���̴�.
	// ������ ĳ������ ȸ�� ������ "���� ��������" ������ �ش�.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// ȸ���� �ε巴�� ����� �ֱ� ���� RotationRate �� �����Ѵ�. ���� �������� ī�޶� ȸ���� ĳ���Ͱ� �ѹ��� ������ ȸ��.
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
