// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/NormalAttack_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UNormalAttack_OnGround::UNormalAttack_OnGround():
	m_CurComboAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_MoveDistance(100.0f)
{
	
}

void UNormalAttack_OnGround::Initialize()
{
	Super::Initialize(); 

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack_OnGround"),
		[this]()
		{
			m_CurComboAttackSection = 1;  
		});
}

bool UNormalAttack_OnGround::GetCanExecuteSkill() const
{
	return !m_Owner->GetIsCrowdControlState() && !m_OwnerSkillComponent->GetIsChargingOnGround();
}

void UNormalAttack_OnGround::Execute()
{
	Super::Execute();
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetSkillState();
	
	if (curSkillState == EMainPlayerSkillStates::Idle)
	{
		m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.

		FVector targetVector = m_Owner->GetActorForwardVector() * m_MoveDistance;
		targetVector.Z = 0.0f;
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		
		m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack_OnGround"));  
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
	}
	else if (curSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||
		curSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
	{
		if (m_OwnerSkillComponent->GetHasStartedComboKeyInputCheck()) // 섹션점프 구간이면,
		{
			m_Owner->RotateActorToKeyInputDirection();

			FVector targetVector = m_Owner->GetActorForwardVector() * m_MoveDistance;
			targetVector.Z = 0.0f;
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
			
			m_OwnerSkillComponent->SetHasStartedComboKeyInputCheck(false);
			
			linqNextNormalAttackOnGroundCombo(); // 섹션점프
		}
	}
}

void UNormalAttack_OnGround::linqNextNormalAttackOnGroundCombo()
{
	if (m_CurComboAttackSection % 2 != 0) // 기본공격중인경우,
	{
		if (m_OwnerSkillComponent->GetIsStrikeAttackActive()) // 강공격키 눌려있으면,
		{
			m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround);
			m_CurComboAttackSection += 1;
		}
		else  // 다음 기본공격 연계
		{
			m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
			m_CurComboAttackSection += 2;
		}
	}
	else // 강공격중인경우,
	{
		if (m_OwnerSkillComponent->GetIsStrikeAttackActive())
		{
			m_CurComboAttackSection = FMath::Clamp(m_CurComboAttackSection + 2, 1, m_MaxNormalAttackSection);
	
			if (m_CurComboAttackSection == 7) // 마지막 공격
			{
				m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
			}
			else
			{
				m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround);
			}
		}
		else
		{
			m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
			m_CurComboAttackSection += 1;
		}
	}
	
	m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("NormalAttack_OnGround"), m_CurComboAttackSection);
}
