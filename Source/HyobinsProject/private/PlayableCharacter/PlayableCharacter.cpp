// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacter/PlayableCharacter.h"
#include "Monster/Monster.h"
#include "MotionWarpingComponent.h"
#include <GameFramework/SpringArmComponent.h.>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "Utility/Utility.h"
#include "SubSystems/UIManager.h"
#include "Algo/MinElement.h"
#include "Component/SkillComponent.h"

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
	m_bIsPressedShift(false),
	m_bIsLockOnMode(false),
	m_LockOnRotaionDuration(0.2f),
	m_LockOnCameraPitch(-37.0f),
	m_OnHitCameraShake(nullptr),
	m_ParryingShake(nullptr),
	m_ParryingSound(nullptr)
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

FVector APlayableCharacter::GetWorldKeyInputDirection(const int32 keyInputDirection) const
{
	// keyInputDirection == 0 ~ 7까지의 8방향. 전방 ~ 좌상.
	
	const float controllerYaw = GetController()->GetControlRotation().Yaw + 45.0f * keyInputDirection;
	const FRotator rotation = {0.0f, controllerYaw, 0.0f};
	
	return rotation.Vector();
}

int32 APlayableCharacter::GetLocalDirectionIndex(const FVector& worldDirection) const
{
	const float forwardDot = FVector::DotProduct(GetActorForwardVector(), worldDirection);
	const float rightDot = FVector::DotProduct(GetActorRightVector(), worldDirection);
	
	constexpr float cos22_5 = 0.382f; // cos(67.5)
	constexpr float cos67_5 = 0.924f; // cos(22.5)
	
	if (rightDot >= 0.0f) // 우측
	{
		if (forwardDot >= cos22_5)
		{
			return 0; // 전방
		}
		
		if (forwardDot >= cos67_5)
		{
			return 1; // 전방 우측 대각
		}
			
		if (forwardDot >= -cos22_5)
		{
			return 2; // 우측
		}
			
		if (forwardDot >= -cos67_5)
		{
			return 3; // 후방 우측 대각
		}

		return 4; // 후방
	}
	
	else // 좌측
	{
		if (forwardDot >= cos22_5)
		{
			return 0; // 전방
		}
		
		if (forwardDot >= cos67_5)
		{
			return 7; // 전방 좌측 대각
		}
			
		if (forwardDot >= -cos22_5)
		{
			return 6; // 좌측
		}
			
		if (forwardDot >= -cos67_5)
		{
			return 5; // 후방 좌측 대각
		}

		return 4; // 후방
	}
}

int32 APlayableCharacter::GetLocalDirectionUsingInverseMatrix(const FVector& worldDirection) const
{
	const FVector localDirection = GetActorTransform().InverseTransformVector(worldDirection);
	const float radian = FMath::Atan2(localDirection.Y, localDirection.X);

	constexpr float range = PI / 8; // 22.5도

	if (radian >= -range && radian < range) // Front
	{
		return 0;
	}

	if (radian >= range && radian < 3 * range) // Front Right
	{
		return 1;
	}
	
	if (radian >= 3 * range && radian < 5 *	range) // Right
	{
		return 2;
	}

	if (radian >= 5 * range && radian < 7 * range) // Back Right
	{
		return 3;
	}

	// if ((radian < -7 * range && radian >= -8 * range) ||
	// 	(radian >= 7 * range && radian <= 8 * range)) // 뒤쪽
	// {
	// 	return 4; 
	// }
	
	if (radian < -5 * range && radian >= -7 * range) // Back Left
	{
		return 5;
	}

	if (radian < -3 * range && radian >= -5 * range) // Left
	{
		return 6;
	}
	
	if (radian < -range && radian >= -3 * range) // Front Left
	{
		return 7;
	}
	
	return 4; // Back
}

UCameraComponent* APlayableCharacter::GetCameraComponent() const
{
	return m_TargetCamera;
}

USkill* APlayableCharacter::GetCurExecutingSkill() const
{
	return m_SkillComponent->GetCurExecutingSkill();
}

void APlayableCharacter::Move(const FInputActionValue& value)
{
	const FVector2d movementVector = value.Get<FVector2D>();
	
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);

	// ForwardVector
	const FVector forwardDireciton = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	// RightVector
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// Add Movement
	this->AddMovementInput(forwardDireciton, movementVector.X);
	this->AddMovementInput(rightDirection, movementVector.Y);

	m_CurInputHorizontal = movementVector.Y;
	m_CurInputVertical = movementVector.X;
}

void APlayableCharacter::Look(const FInputActionValue& value)
{
	if (!m_bIsLockOnMode)
	{
		const FVector2d axisVector = value.Get<FVector2d>();

		this->AddControllerYawInput(axisVector.X);
		this->AddControllerPitchInput(axisVector.Y);
	}
}

void APlayableCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = m_RunSpeed;
	m_bIsPressedShift = true;
}

void APlayableCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
	m_bIsPressedShift = false;
}

void APlayableCharacter::ToggleLockOnMode()
{
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	
	if (m_bIsLockOnMode)
	{
		m_bIsLockOnMode = false;
		GetWorldTimerManager().ClearTimer(m_LockOnTimer);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		uiManager->SetVisibilityWidgets(TEXT("LockOn"), ESlateVisibility::Collapsed);
	}
	else // 락온모드 키려할 경우
	{
		m_CurLockOnTarget = findNearestTarget();

		if (m_CurLockOnTarget != nullptr)
		{
			m_bIsLockOnMode = true;
			lockOnToTarget(m_CurLockOnTarget.Get());
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		
			uiManager->RenderLockOnToScreen(m_CurLockOnTarget.Get());
		}
	}
}

ACharacterBase* APlayableCharacter::findNearestTarget()
{
	const FVector startLocation = this->GetActorLocation();
	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
	const TArray<AActor*> ignoreActors = {this};
	TArray<AActor*> overlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(this->GetWorld(),
		startLocation,
		1500.0f, // 구체 반지름
		objectTypes,
		nullptr,
		ignoreActors,
		overlappedActors);

	AActor* nearestEnemy = nullptr;
	
	if (overlappedActors.Num() > 0)
	{
		nearestEnemy = *Algo::MinElementBy(
		overlappedActors,
	[this](const AActor* Actor)
		{
			return FVector::DistSquared(this->GetActorLocation(), Actor->GetActorLocation());
		});
		
	}	
	
	return Cast<ACharacterBase>(nearestEnemy);
}

void APlayableCharacter::lockOnToTarget(AActor* target)
{
	if (target == nullptr)
	{
		return;
	}
	
	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (playerController != nullptr)
	{
		m_InitialControlRotation = playerController->GetControlRotation();
		m_LockOnElapsed = 0.0f;

		constexpr float updateRate = 1 / 60.0f;
		
		GetWorldTimerManager().SetTimer(m_LockOnTimer, this, &APlayableCharacter::updateCameraRotation, updateRate, true);
	}
}

void APlayableCharacter::updateCameraRotation()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController == nullptr || !m_CurLockOnTarget.IsValid() || m_CurLockOnTarget->IsDead())
	{
		ToggleLockOnMode();
		return;
	}

	m_LockOnElapsed += 0.01f;
	float alpha = FMath::Clamp(m_LockOnElapsed / m_LockOnRotaionDuration, 0.0f, 1.0f);
							 
	FVector cameraLocation = m_TargetCamera->GetComponentLocation();
	FVector targetLocation = m_CurLockOnTarget->GetActorLocation();
	FVector directionToTarget = (targetLocation - cameraLocation).GetSafeNormal();

	FRotator targetControlRotation = directionToTarget.Rotation();
	targetControlRotation.Roll = 0.0f;
	targetControlRotation.Pitch = m_LockOnCameraPitch;
	
	FRotator newRotation = FMath::RInterpTo(m_InitialControlRotation, targetControlRotation, alpha, 1.0f);
	
	playerController->SetControlRotation(newRotation);
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


