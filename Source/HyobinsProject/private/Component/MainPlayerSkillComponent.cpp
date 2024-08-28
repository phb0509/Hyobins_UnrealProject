// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "MotionWarpingComponent.h"
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"

const FName UMainPlayerSkillComponent::NormalAttackName = "NormalAttack";
const FName UMainPlayerSkillComponent::NormalStrikeAttackName = "NormalStrikeAttack";
const FName UMainPlayerSkillComponent::Dodge_NonTargeting = "Dodge_NonTargeting";
const FName UMainPlayerSkillComponent::Dodge_Targeting = "Dodge_Targeting";


UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_bHasStartedComboKeyInputCheck(false),
	m_CurNormalAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_bHasleftShiftDecision(false),
	m_bSucceededParrying(false)
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
	if (m_CurSkillState == EMainPlayerSkillStates::Idle) 
	{
		m_OwnerAnimInstance->PlayMontage(NormalAttackName, 1.1f);
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

void UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_Start_EachSection() // 각 공격섹션 시작 시 호출.
{
	if (m_CurNormalAttackSection % 2 != 0)
	{
		m_CurSkillState = EMainPlayerSkillStates::NormalAttack;
	}
	else
	{
		m_CurSkillState = EMainPlayerSkillStates::NormalStrikeAttack;
	}
	
	m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		FName("Forward"), m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * 120.0f);
}

void UMainPlayerSkillComponent::linqNextNormalAttackCombo()
{
	if (m_CurNormalAttackSection % 2 != 0) // 기본공격중인경우,
	{
		if (m_Owner->GetIsPressedKey("LeftShift") || m_bHasleftShiftDecision) // 강공격키 눌려있으면,
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
		if (m_Owner->GetIsPressedKey("LeftShift") || m_bHasleftShiftDecision)
		{
			m_CurNormalAttackSection = FMath::Clamp(m_CurNormalAttackSection + 2, 1, m_MaxNormalAttackSection);
		}
		else
		{
			m_CurNormalAttackSection += 1;
		}
	}
	
	m_OwnerAnimInstance->JumpToMontageSection(NormalAttackName, m_CurNormalAttackSection);
}

void UMainPlayerSkillComponent::UpperAttack()
{
	if (m_CurSkillState == EMainPlayerSkillStates::Idle ||
		m_CurSkillState == EMainPlayerSkillStates::NormalAttack ||
		m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack)
	{
		m_CurSkillState = EMainPlayerSkillStates::UpperAttack;
		m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.
		m_OwnerAnimInstance->PlayMontage("UpperAttack");
		
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		FName("Forward"), m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * 100.0f);
	}
}

void UMainPlayerSkillComponent::Dodge()
{
	if (m_CurSkillState != EMainPlayerSkillStates::Dodge_NonTargeting &&
		m_CurSkillState != EMainPlayerSkillStates::Idle) // 어떠한 공격이든 수행중이면
	{
		
		m_Owner->RotateActorToControllerYaw(); // 카메라 정면(컨트롤러의 Yaw로 회전)
		m_OwnerAnimInstance->StopAllMontages(0.0f); // 빠른 모션변환을 위해 현재 재생중인 몽타주 Stop.

		int32 combatDodgeSection = Utility::GetDirectionIndex(m_Owner->GetCurInputVertical(), m_Owner->GetCurInputHorizontal());
		m_OwnerAnimInstance->PlayMontage(Dodge_NonTargeting,1.2f);
		m_OwnerAnimInstance->JumpToMontageSection(Dodge_NonTargeting, combatDodgeSection);
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
	m_OwnerAnimInstance->NormalAttack_Start_EachSection.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_Start_EachSection);
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(NormalAttackName,
	[this](){m_CurNormalAttackSection = 1;});
	
	// CombatDodge_OnGround
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageStarted(Dodge_NonTargeting,
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