// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "MotionWarpingComponent.h"
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "MatineeCameraShake.h"
#include "Kismet/GameplayStatics.h"


UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_GravityScaleInAir(0.00001f),
	m_bHasStartedComboKeyInputCheck(false),
	m_CurComboAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_bIsStrikeAttackActive(false),
	m_StrikeAttackDecisionTime(0.5f),
	m_NormalAttackOnGroundMoveDistance(120.0f),
	m_UpperAttackGroundToAirJumpDistance(300.0f),
	m_DashAttackOnGroundMoveDistance(500.0f),
	m_EarthStrikeEffect(nullptr),
	m_EarthStrikeSound(nullptr),
	m_DodgeOnGroundMoveDistance(400.0f),
	m_ChargingComboDashAttackOnGroundMoveDistance(700.0f)
{
	PrimaryComponentTick.bCanEverTick = true; // �α���¿�
}

void UMainPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Owner = Cast<AMainPlayer>(GetOwner());
	m_OwnerAnimInstance = Cast<UMainPlayerAnim>(m_Owner->GetMesh()->GetAnimInstance());
	
	bindFuncOnMontageEvent();
}

void UMainPlayerSkillComponent::NormalAttack_OnGround()
{
	if (m_CurSkillState == EMainPlayerSkillStates::Idle)
	{
		m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack_OnGround"));
		m_CurSkillState = EMainPlayerSkillStates::NormalAttack_OnGround;
		
		m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.
		
		FVector targetVector = m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance;
		targetVector.Z = 0.0f;
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
	}
	else if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
	{
		if (m_bHasStartedComboKeyInputCheck) // �������� �����̸�,
		{
			m_bHasStartedComboKeyInputCheck = false;
			linqNextNormalAttackOnGroundCombo(); // ��������

			m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.

			FVector targetVector = m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance;
			targetVector.Z = 0.0f;
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		}
	}
}

void UMainPlayerSkillComponent::linqNextNormalAttackOnGroundCombo()
{
	if (m_CurComboAttackSection % 2 != 0) // �⺻�������ΰ��,
	{
		if (m_bIsStrikeAttackActive) // ������Ű ����������,
		{
			m_CurSkillState = EMainPlayerSkillStates::NormalStrikeAttack_OnGround;
			m_CurComboAttackSection += 1;
		}
		else  // ���� �⺻���� ����
		{
			m_CurSkillState = EMainPlayerSkillStates::NormalAttack_OnGround;
			m_CurComboAttackSection += 2;
		}
	}
	else // ���������ΰ��,
	{
		if (m_bIsStrikeAttackActive)
		{
			m_CurComboAttackSection = FMath::Clamp(m_CurComboAttackSection + 2, 1, m_MaxNormalAttackSection);

			if (m_CurComboAttackSection == 7) // ������ ����
			{
				m_CurSkillState = EMainPlayerSkillStates::NormalAttack_OnGround;
			}
			else
			{
				m_CurSkillState = EMainPlayerSkillStates::NormalStrikeAttack_OnGround;
			}
		}
		else
		{
			m_CurSkillState = EMainPlayerSkillStates::NormalAttack_OnGround;
			m_CurComboAttackSection += 1;
		}
	}
	
	m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("NormalAttack_OnGround"), m_CurComboAttackSection);
}

void UMainPlayerSkillComponent::NormalAttack_InAir()
{
	if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack_InAir)
	{
		if (m_bHasStartedComboKeyInputCheck) // �������� �����̸�,
		{
			m_bHasStartedComboKeyInputCheck = false;
			linqNextNormalAttackInAirCombo(); // ��������

			m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.
    
			FVector targetVector = m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance;
			targetVector.Z = 0.0f;
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		}
	}
	else
	{
		if (m_CurSkillState != EMainPlayerSkillStates::EarthStrike_FallingToGround &&
			m_CurSkillState == EMainPlayerSkillStates::Idle)
		{
			m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack_InAir"));

			m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.

			FVector targetVector = m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance;
			targetVector.Z = 0.0f;
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		}
	}
}

void UMainPlayerSkillComponent::linqNextNormalAttackInAirCombo()
{
	m_CurComboAttackSection += 1;
	m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("NormalAttack_InAir"), m_CurComboAttackSection);
}

void UMainPlayerSkillComponent::UpperAttack_OnGround()
{
	if (m_CurSkillState == EMainPlayerSkillStates::Idle ||
		m_CurSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
	{
		m_Owner->RotateActorToKeyInputDirection();

		if (m_bIsStrikeAttackActive)
		{
			m_CurSkillState = EMainPlayerSkillStates::UpperAttack_GroundToAir;
			m_OwnerAnimInstance->PlayMontage(TEXT("UpperAttack_GroundToAir"));
			m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying); // Flying���� �ؾ� ��ǿ����� z���̵�.

			const FVector targetLocation = m_Owner->GetActorLocation() +
				(m_Owner->GetActorUpVector() * m_UpperAttackGroundToAirJumpDistance) +
				(m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance);
			
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), targetLocation);
		}
		else
		{
			m_CurSkillState = EMainPlayerSkillStates::UpperAttack_OnGround;
			m_OwnerAnimInstance->PlayMontage("UpperAttack_OnGround");

			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"),
				m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance);
		}
	}
}

void UMainPlayerSkillComponent::DashAttack_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState != EMainPlayerSkillStates::DashAttack_OnGround)
		{
			m_CurSkillState = EMainPlayerSkillStates::DashAttack_OnGround;
			m_Owner->RotateActorToKeyInputDirection();
			m_OwnerAnimInstance->PlayMontage("DashAttack_OnGround");

			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"),
				m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_DashAttackOnGroundMoveDistance);
		}
	}
}

void UMainPlayerSkillComponent::DashAttack_InAir()
{
	if (m_CurSkillState != EMainPlayerSkillStates::DashAttack_InAir &&
		m_CurSkillState != EMainPlayerSkillStates::EarthStrike_FallingToGround)
	{
		m_Owner->GetCharacterMovement()->GravityScale = m_GravityScaleInAir;

		m_CurSkillState = EMainPlayerSkillStates::DashAttack_InAir;
		m_Owner->RotateActorToKeyInputDirection();
		m_OwnerAnimInstance->PlayMontage("DashAttack_InAir");

		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"),
			m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_DashAttackOnGroundMoveDistance);
	}
}


void UMainPlayerSkillComponent::Dodge_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState != EMainPlayerSkillStates::Dodge_OnGround &&
		m_CurSkillState != EMainPlayerSkillStates::Idle) // ��� �����̵� �������̸�
		{
			m_CurSkillState = EMainPlayerSkillStates::Dodge_OnGround;
			
			m_OwnerAnimInstance->StopAllMontages(0.0f); 
			
			// Ű�Է¿� ���� ��Ʈ�ѷ����⺤�� ���ϱ�.
			const FVector controllerKeyInputDirection = m_Owner->GetControllerKeyInputDirectionVector(
				m_Owner->GetDirectionIndexFromKeyInput());

			// ��Ʈ�ѷ����⺤�͸� ĳ���� ���ú��ͷ� ��ȯ�� ĳ���ͱ��� ���� �˾ƿ���.
			const int32 localDirection = m_Owner->GetLocalDirection(controllerKeyInputDirection);
			
			m_OwnerAnimInstance->PlayMontage(TEXT("Dodge_OnGround"));
			m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("Dodge_OnGround"), localDirection);
			
			const FVector targetVerticalVector = m_Owner->GetForwardVectorFromControllerYaw() * m_DodgeOnGroundMoveDistance * m_Owner->GetCurInputVertical();
			const FVector targetHorizontalVector = m_Owner->GetRightVectorFromControllerYaw() * m_DodgeOnGroundMoveDistance * m_Owner->GetCurInputHorizontal();
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVerticalVector + targetHorizontalVector);
		}
	}
}


void UMainPlayerSkillComponent::ExtendStrikeActivateDecisionTime()
{
	FTimerHandle timer;
	m_Owner->GetWorldTimerManager().SetTimer(timer,
	[this]()
	{
		m_bIsStrikeAttackActive = false;
	},
	m_StrikeAttackDecisionTime,
	false);
}

void UMainPlayerSkillComponent::EarthStrike_InAir()
{
	if (m_CurSkillState != EMainPlayerSkillStates::EarthStrike_FallingToGround)
	{
		m_CurSkillState = EMainPlayerSkillStates::EarthStrike_FallingToGround;
		
		m_OwnerAnimInstance->PlayMontage(TEXT("EarthStrike_FallingToGround"));
		m_Owner->GetCharacterMovement()->GravityScale = 6.0f;
		m_Owner->GetWorldTimerManager().SetTimer
			(
				m_EarthStrikeTimer,
				[this]()
				{
					if (m_Owner->GetIsOnGround())
					{
						EarthStrike_OnGround();
						EarthStrike_HitCheck();
					}
				},
			GetWorld()->DeltaTimeSeconds,
			true,-1);
	}
}

void UMainPlayerSkillComponent::EarthStrike_OnGround()
{
	if (m_CurSkillState != EMainPlayerSkillStates::EarthStrike_OnGround)
	{
		m_Owner->GetWorldTimerManager().ClearTimer(m_EarthStrikeTimer);
		m_OwnerAnimInstance->PlayMontage(TEXT("EarthStrike_OnGround"));
	}
}

void UMainPlayerSkillComponent::EarthStrike_HitCheck()
{
	const FVector startLocation = m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation();
	
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
	const TArray<AActor*> IgnoreActors = {m_Owner.Get()};
	TArray<AActor*> overlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		startLocation,
		500.0f, // ��ü ������
		ObjectTypes,
		nullptr,
		IgnoreActors,
		overlappedActors);

	if (overlappedActors.Num() > 0)
	{
		for (AActor* overlappedEnemy : overlappedActors)
		{
			const ACharacterBase* enemy = Cast<ACharacterBase>(overlappedEnemy);
			if (enemy->GetIsOnGround())
			{
				m_Owner->Attack(TEXT("EarthStrike"), overlappedEnemy);
			}
		}
	}
	
	EarthStrike_PlayEffect();
	UGameplayStatics::PlaySound2D(this, m_EarthStrikeSound, 1.0f);
}

void UMainPlayerSkillComponent::EarthStrike_PlayEffect()
{
	if (m_EarthStrikeEffect != nullptr)
	{
		const UNiagaraComponent* niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_EarthStrikeEffect,
		m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation(),
		FRotator(0.0f),FVector(1.0f, 1.0f, 1.0f)
		);
	}
	
	// Camera Shake
	if (m_EarthStrikeCameraShake != nullptr)
	{
		m_Owner->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_EarthStrikeCameraShake);
	}
}

void UMainPlayerSkillComponent::Charging_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState != EMainPlayerSkillStates::Charging_OnGround)
		{
			m_CurSkillState = EMainPlayerSkillStates::Charging_OnGround;
			m_OwnerAnimInstance->PlayMontage(TEXT("Charging_OnGround"));

			m_Owner->AddInputContextMappingOnCharging();
		}
	}
}

void UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState == EMainPlayerSkillStates::Charging_OnGround)
		{
			m_OwnerAnimInstance->PlayMontage(TEXT("Charging_ComboDashAttack_OnGround"));
			m_CurSkillState = EMainPlayerSkillStates::Charging_ComboDashAttack_OnGround;

			//m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.
			//m_Owner->RotateActorToControllerYaw();

			// FVector targetVector = m_Owner->GetActorForwardVector() * m_ChargingComboDashAttackOnGroundMoveDistance;
			// targetVector.Z = 0.0f;
			// m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			// 	TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		}
		else if (m_CurSkillState == EMainPlayerSkillStates::Charging_ComboDashAttack_OnGround)
		{
			// linqNextChargingComboDashAttackOnGroundCombo();
			//
			// //m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.
			// m_Owner->RotateActorToControllerYaw();
			//
			// FVector targetVector = m_Owner->GetActorForwardVector() * m_ChargingComboDashAttackOnGroundMoveDistance;
			// targetVector.Z = 0.0f;
			// m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			// 	TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		}
	}
}

void UMainPlayerSkillComponent::linqNextChargingComboDashAttackOnGroundCombo()
{
	// m_CurComboAttackSection += 1;
	// m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("Charging_ComboDashAttack_OnGround"), m_CurComboAttackSection);
}

void UMainPlayerSkillComponent::initGravityScaleAfterAttack()
{
	if (m_Owner->GetCharacterMovement()->GravityScale == m_GravityScaleInAir)
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	}
}

void UMainPlayerSkillComponent::SetIdle(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		m_CurSkillState = EMainPlayerSkillStates::Idle;
	}
}

void UMainPlayerSkillComponent::bindFuncOnMontageEvent()
{
	// NormalAttack_OnGround
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack_OnGround"),
[this]()
	{
		m_CurComboAttackSection = 1;
	});
	
	
	// UpperAttack_GroundToAir
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("UpperAttack_GroundToAir"),
	[this]()
	{
		m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	});

	
	// NormalAttack_InAir
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageStarted(TEXT("NormalAttack_InAir"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = m_GravityScaleInAir;
		m_CurSkillState = EMainPlayerSkillStates::NormalAttack_InAir;
	});
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("NormalAttack_InAir"),
	[this]()
	{
		initGravityScaleAfterAttack();
	});

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack_InAir"),
[this]()
	{
		m_CurComboAttackSection = 1;
	});

	
	// DashAttack_InAir
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("DashAttack_InAir"),
	[this]()
	{
		initGravityScaleAfterAttack();
	});

	// EarthStrike_OnGround
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("EarthStrike_OnGround"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	});

	// Charging
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Charging_OnGround"),
[this]()
	{
		//m_Owner->RemoveInputContextMappingOnCharging();
	});

	
	// Charging_ComboDashAttack_OnGround
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Charging_ComboDashAttack_OnGround"),
[this]()
	{
		//m_CurComboAttackSection = 1;
		m_Owner->RemoveInputContextMappingOnCharging();
	});
	
	// OnMontageEnded 
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle); // � ��ų�̵� '������(not Interrupted)' ��� �� Idle�� ��ȯ.
}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // �α���¿� ƽ
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	GEngine->AddOnScreenDebugMessage(50, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), m_CurComboAttackSection));
	GEngine->AddOnScreenDebugMessage(51, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(52, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_bIsStrikeAttackActive));
	GEngine->AddOnScreenDebugMessage(53, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
}