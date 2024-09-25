// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "MotionWarpingComponent.h"
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MatineeCameraShake.h"
#include "Kismet/GameplayStatics.h"


UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_bHasStartedComboKeyInputCheck(false),
	m_CurNormalAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_bHasleftShiftDecision(false),
	m_NormalAttackOnGroundMoveDistance(120.0f),
	m_UpperAttackGroundToAirJumpDistance(300.0f),
	m_DashAttackOnGroundMoveDistance(500.0f),
	m_EarthStrikeEffect(nullptr),
	m_EarthStrikeSound(nullptr),
	m_DodgeOnGroundMoveDistance(400.0f),
	m_GravityScaleInAir(0.00001f)
{
	PrimaryComponentTick.bCanEverTick = true; // 로그출력용
	
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
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState == EMainPlayerSkillStates::Idle) 
		{
			m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack_OnGround"));
		}
		else if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||    
				m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
		{
			if (m_bHasStartedComboKeyInputCheck) // 섹션점프 구간이면,
			{
				m_bHasStartedComboKeyInputCheck = false;
				linqNextNormalAttackOnGroundCombo(); // 섹션점프
			}
		}
	}
}

void UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_Start_EachSection() // 각 공격섹션 시작 시 호출.
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurNormalAttackSection % 2 != 0)
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
		m_Owner->GetCharacterMovement()->GravityScale = m_GravityScaleInAir;
		m_CurSkillState = EMainPlayerSkillStates::NormalAttack_InAir;
	}
	
	m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.

	FVector targetVector = m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance;
	targetVector.Z = 0.0f;
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
}

void UMainPlayerSkillComponent::linqNextNormalAttackOnGroundCombo()
{
	if (m_CurNormalAttackSection % 2 != 0) // 기본공격중인경우,
	{
		if (m_Owner->GetIsPressedKey(TEXT("LeftShift")) || m_bHasleftShiftDecision) // 강공격키 눌려있으면,
		{
			m_CurNormalAttackSection += 1;
		}
		else  // 다음 기본공격 연계
		{
			m_CurNormalAttackSection += 2;
		}
	}
	else // 강공격중인경우,
	{
		if (m_Owner->GetIsPressedKey(TEXT("LeftShift")) || m_bHasleftShiftDecision)
		{
			m_CurNormalAttackSection = FMath::Clamp(m_CurNormalAttackSection + 2, 1, m_MaxNormalAttackSection);
		}
		else
		{
			m_CurNormalAttackSection += 1;
		}
	}
	
	m_OwnerAnimInstance->JumpToMontageSection(TEXT("NormalAttack_OnGround"), m_CurNormalAttackSection);
}

void UMainPlayerSkillComponent::NormalAttack_InAir()
{
	if (m_Owner->GetIsFalling() ||
		m_Owner->GetIsFlying())
	{
		if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack_InAir)
		{
			if (m_bHasStartedComboKeyInputCheck) // 섹션점프 구간이면,
			{
				m_bHasStartedComboKeyInputCheck = false;
				linqNextNormalAttackInAirCombo(); // 섹션점프
			}
		}
		else
		{
			if (m_CurSkillState != EMainPlayerSkillStates::EarthStrike_FallingToGround)
			{
				m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack_InAir"));
			}
		}
	}
}

void UMainPlayerSkillComponent::linqNextNormalAttackInAirCombo()
{
	m_CurNormalAttackSection += 1;
	m_OwnerAnimInstance->JumpToMontageSection(TEXT("NormalAttack_InAir"), m_CurNormalAttackSection);
}

void UMainPlayerSkillComponent::initGravityScaleAfterAttack()
{
	if (m_Owner->GetCharacterMovement()->GravityScale == m_GravityScaleInAir)
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	}
}

void UMainPlayerSkillComponent::UpperAttack_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState == EMainPlayerSkillStates::Idle ||
		m_CurSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
		{
			m_Owner->RotateActorToKeyInputDirection(); 
		
			if (m_Owner->GetIsPressedKey(TEXT("LeftShift")))
			{
				m_CurSkillState = EMainPlayerSkillStates::UpperAttack_GroundToAir;
				m_OwnerAnimInstance->PlayMontage(TEXT("UpperAttack_GroundToAir"));
				m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying); // Flying모드로 해야 모션워핑이 z축이동.

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
				TEXT("Forward"), m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_NormalAttackOnGroundMoveDistance);
			}
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
					TEXT("Forward"), m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_DashAttackOnGroundMoveDistance);
		}
	}
}

void UMainPlayerSkillComponent::DashAttack_InAir()
{
	if (!m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState != EMainPlayerSkillStates::DashAttack_InAir &&
			m_CurSkillState != EMainPlayerSkillStates::EarthStrike_FallingToGround)
		{
			m_Owner->GetCharacterMovement()->GravityScale = m_GravityScaleInAir;
		
			m_CurSkillState = EMainPlayerSkillStates::DashAttack_InAir;
			m_Owner->RotateActorToKeyInputDirection(); 
			m_OwnerAnimInstance->PlayMontage("DashAttack_InAir");
    		
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
					TEXT("Forward"), m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_DashAttackOnGroundMoveDistance);
		}
	}
}


void UMainPlayerSkillComponent::Dodge_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState != EMainPlayerSkillStates::Dodge_OnGround &&
		m_CurSkillState != EMainPlayerSkillStates::Idle) // 어떠한 공격이든 수행중이면
		{
			m_Owner->RotateActorToControllerYaw(); // 카메라 정면(컨트롤러의 Yaw로 회전)
			m_OwnerAnimInstance->StopAllMontages(0.0f); 

			const int32 combatDodgeSection = Utility::GetDirectionIndex(m_Owner->GetCurInputVertical(), m_Owner->GetCurInputHorizontal());
			m_OwnerAnimInstance->PlayMontage(TEXT("Dodge_OnGround"));
			m_OwnerAnimInstance->JumpToMontageSection(TEXT("Dodge_OnGround"), combatDodgeSection);

			const FVector targetVerticalVector = m_Owner->GetActorForwardVector() * m_DodgeOnGroundMoveDistance * m_Owner->GetCurInputVertical();
			const FVector targetHorizontalVector = m_Owner->GetActorRightVector() * m_DodgeOnGroundMoveDistance * m_Owner->GetCurInputHorizontal();
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVerticalVector + targetHorizontalVector);
		}
	}
}


void UMainPlayerSkillComponent::ExtendShiftDecisionTime()
{
	if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
	{
		m_bHasleftShiftDecision = true;
	
		m_Owner->GetWorldTimerManager().SetTimer
			(
				m_ShiftDecisionTimerHandle,
				[this]()
					{ m_bHasleftShiftDecision = false; },
			0.1f,
			false);
	}
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
	FVector startLocation = m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation();
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> IgnoreActors = {m_Owner.Get()};
	TArray<AActor*> overlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		startLocation,
		500.0f, // 구체 반지름
		ObjectTypes,
		nullptr,
		IgnoreActors,
		overlappedActors);

	if (overlappedActors.Num() > 0)
	{
		for (AActor* overlappedEnemy : overlappedActors)
		{
			m_Owner->Attack(TEXT("EarthStrike"), overlappedEnemy);
		}
	}
	
	EarthStrike_PlayEffect();
	UGameplayStatics::PlaySound2D(this, m_EarthStrikeSound, 1.0f);
}

void UMainPlayerSkillComponent::EarthStrike_PlayEffect()
{
	if (m_EarthStrikeEffect != nullptr)
	{
		UNiagaraComponent* niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_EarthStrikeEffect,
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


void UMainPlayerSkillComponent::SetIdle(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		m_CurSkillState = EMainPlayerSkillStates::Idle;
	}
}

void UMainPlayerSkillComponent::bindFuncOnMontageEvent()
{
	// Attack_OnGround
	m_OwnerAnimInstance->NormalAttack_Start_EachSection.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_Start_EachSection);
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("NormalAttack_OnGround"),
	[this]() { m_CurNormalAttackSection = 1; });

	// Attack_InAir
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("UpperAttack_GroundToAir"),
	[this]()
	{
		m_Owner->GetCharacterMovement()->SetMovementMode(
		MOVE_Falling);
	});
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("NormalAttack_InAir"),
	[this]()
	{
		m_CurNormalAttackSection = 1;
		initGravityScaleAfterAttack();
	});

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("DashAttack_InAir"),
	[this]()
	{
		initGravityScaleAfterAttack();
	});

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("EarthStrike_OnGround"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	});

	
	// CombatDodge_OnGround
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageStarted(TEXT("Dodge_OnGround"),
		[this]()
		{
			m_CurSkillState = EMainPlayerSkillStates::Dodge_OnGround;
		});
	
	// OnMontageEnded
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle);
}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // 로그출력용 틱
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	GEngine->AddOnScreenDebugMessage(50, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), m_CurNormalAttackSection));
	GEngine->AddOnScreenDebugMessage(51, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(52, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_bHasleftShiftDecision || m_Owner->GetIsPressedKey("LeftShift")));
	GEngine->AddOnScreenDebugMessage(53, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
}