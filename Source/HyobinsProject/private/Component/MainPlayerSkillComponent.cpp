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


UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_bHasStartedComboKeyInputCheck(false),
	m_CurNormalAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_bHasleftShiftDecision(false),
	m_NormalAttackMoveDistance(120.0f),
	m_UpperAttackToAirJumpDistance(300.0f),
	m_DodgeMoveDistance(400.0f)
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

void UMainPlayerSkillComponent::NormalAttack()
{
	if (m_Owner->GetMovementComponent()->IsMovingOnGround())
	{
		NormalAttack_OnGround();
	}
	else if (m_Owner->GetIsFalling() ||
		m_Owner->GetIsFlying())
	{
		NormalAttack_InAir();
	}
}

void UMainPlayerSkillComponent::NormalAttack_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState == EMainPlayerSkillStates::Idle) 
		{
			m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack"));
		}
		else if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack ||    
				m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack)
		{
			if (m_bHasStartedComboKeyInputCheck) // 섹션점프 구간이면,
			{
				m_bHasStartedComboKeyInputCheck = false;
				linqNextNormalAttackCombo(); // 섹션점프
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
			m_CurSkillState = EMainPlayerSkillStates::NormalAttack;
		}
		else
		{
			m_CurSkillState = EMainPlayerSkillStates::NormalStrikeAttack;
		}
	}
	else
	{
		m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		m_CurSkillState = EMainPlayerSkillStates::NormalAttackInAir;
	}
	
	m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.

	FVector targetVector = m_Owner->GetActorForwardVector() * m_NormalAttackMoveDistance;
	targetVector.Z = 0.0f;
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
}

void UMainPlayerSkillComponent::linqNextNormalAttackCombo()
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
	
	m_OwnerAnimInstance->JumpToMontageSection(TEXT("NormalAttack"), m_CurNormalAttackSection);
}

void UMainPlayerSkillComponent::NormalAttack_InAir()
{

	if (m_Owner->GetIsFalling() ||
		m_Owner->GetIsFlying())
	{
		if (m_CurSkillState == EMainPlayerSkillStates::NormalAttackInAir)
		{
			if (m_bHasStartedComboKeyInputCheck) // 섹션점프 구간이면,
			{
				m_bHasStartedComboKeyInputCheck = false;
				linqNextNormalAttackInAirCombo(); // 섹션점프
			}
		}
		else
		{
			m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttackInAir"));
		}
	}
}

void UMainPlayerSkillComponent::linqNextNormalAttackInAirCombo()
{
	// 쉬프트키누르면 바로 땅내려직게해야됨.
	m_CurNormalAttackSection += 1;
	m_OwnerAnimInstance->JumpToMontageSection(TEXT("NormalAttackInAir"), m_CurNormalAttackSection);
}

void UMainPlayerSkillComponent::UpperAttack()
{
	if (m_CurSkillState == EMainPlayerSkillStates::Idle ||
		m_CurSkillState == EMainPlayerSkillStates::NormalAttack ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack)
	{
		if (m_Owner->GetIsPressedKey(TEXT("LeftShift")))
		{
			m_CurSkillState = EMainPlayerSkillStates::UpperAttackToAir;
			m_OwnerAnimInstance->PlayMontage(TEXT("UpperAttackToAir"));

			m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

			const FVector targetLocation = m_Owner->GetActorLocation() +
				(m_Owner->GetActorUpVector() * m_UpperAttackToAirJumpDistance) +
					(m_Owner->GetActorForwardVector() * m_NormalAttackMoveDistance);
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), targetLocation);
		}
		else
		{
			m_CurSkillState = EMainPlayerSkillStates::UpperAttack;
			m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.
			m_OwnerAnimInstance->PlayMontage("UpperAttack");
			
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_NormalAttackMoveDistance);
		}
	}
}



void UMainPlayerSkillComponent::Dodge()
{
	if (m_Owner->GetIsOnGround())
	{
		if (m_CurSkillState != EMainPlayerSkillStates::Dodge_NonTargeting &&
		m_CurSkillState != EMainPlayerSkillStates::Idle) // 어떠한 공격이든 수행중이면
		{
			m_Owner->RotateActorToControllerYaw(); // 카메라 정면(컨트롤러의 Yaw로 회전)
			m_OwnerAnimInstance->StopAllMontages(0.0f); // 빠른 모션변환을 위해 현재 재생중인 몽타주 Stop.

			const int32 combatDodgeSection = Utility::GetDirectionIndex(m_Owner->GetCurInputVertical(), m_Owner->GetCurInputHorizontal());
			m_OwnerAnimInstance->PlayMontage(TEXT("Dodge_NonTargeting"));
			m_OwnerAnimInstance->JumpToMontageSection(TEXT("Dodge_NonTargeting"), combatDodgeSection);

			const FVector targetVerticalVector = m_Owner->GetActorForwardVector() * m_DodgeMoveDistance * m_Owner->GetCurInputVertical();
			const FVector targetHorizontalVector = m_Owner->GetActorRightVector() * m_DodgeMoveDistance * m_Owner->GetCurInputHorizontal();
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVerticalVector + targetHorizontalVector);
		}
	}
}


void UMainPlayerSkillComponent::ExtendShiftDecisionTime()
{
	if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack)
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

void UMainPlayerSkillComponent::AirToGroundAttack_InAir()
{
	m_OwnerAnimInstance->PlayMontage(TEXT("AirToGroundAttack_InAir"));
	m_Owner->GetCharacterMovement()->GravityScale = 6.0f;
	m_Owner->GetWorldTimerManager().SetTimer
		(
			m_AirToGroundAttackTimer,
			[this]()
			{
				if (m_Owner->GetIsOnGround())
				{
					AirToGroundAttack_OnGround();
					AirToGroundAttack_CollisionCheck();
				}
			},
		GetWorld()->DeltaTimeSeconds,
		true,-1);
}

void UMainPlayerSkillComponent::AirToGroundAttack_OnGround()
{
	m_Owner->GetWorldTimerManager().ClearTimer(m_AirToGroundAttackTimer);
	m_OwnerAnimInstance->PlayMontage(TEXT("AirToGroundAttack_OnGround"));
}

void UMainPlayerSkillComponent::AirToGroundAttack_CollisionCheck()
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
			m_Owner->Attack(TEXT("AirToGroundAttack"), overlappedEnemy);
		}
	}
	
	AirToGroundAttack_PlayEffect();
}

void UMainPlayerSkillComponent::AirToGroundAttack_PlayEffect()
{
	UNiagaraComponent* niagaraComponent;
	
	if (m_AirToGroundAttackEffect != nullptr)
	{
		niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_AirToGroundAttackEffect,
		m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation(),
		FRotator(0.0f),FVector(1.0f, 1.0f, 1.0f)
		);
	}

	FTimerHandle destroyTimer;
	m_Owner->GetWorldTimerManager().SetTimer
		(
			destroyTimer,
			[=]()
			{ niagaraComponent->DeactivateImmediate(); },
		3.0f,
		false);

	// Camera Shake
	if (m_AirToGroundAttackCameraShake != nullptr)
	{
		m_Owner->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_AirToGroundAttackCameraShake);
		int tt =0;
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
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("NormalAttack"),
	[this]() { m_CurNormalAttackSection = 1; });

	// Attack_InAir
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("UpperAttackToAir"),
	[this]()
	{
		m_Owner->GetCharacterMovement()->SetMovementMode(
		MOVE_Falling);
	});
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("NormalAttackInAir"),
	[this]()
	{
		m_CurNormalAttackSection = 1;
		m_Owner->GetCharacterMovement()->SetMovementMode(
		MOVE_Falling);
	});

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(TEXT("AirToGroundAttack_OnGround"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	});

	
	// CombatDodge_OnGround
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageStarted(TEXT("Dodge_NonTargeting"),
		[this]()
		{
			m_CurSkillState = EMainPlayerSkillStates::Dodge_NonTargeting;
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