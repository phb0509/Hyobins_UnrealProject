// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/NormalAttack_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UNormalAttack_OnGround::UNormalAttack_OnGround():
	m_NormalAttackMontage(nullptr),
	m_CurComboAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_MoveDistance(100.0f)
{

}

void UNormalAttack_OnGround::Initialize()
{
	Super::Initialize(); 

	check(m_NormalAttackMontage != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack_OnGround"),
		[this]()
		{
			m_CurComboAttackSection = 1;  
		});
}

void UNormalAttack_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	check(ownerSkillComponent != nullptr);
	
	if (m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::None))
	{
		FVector targetVector = m_Owner->GetActorForwardVector() * m_MoveDistance;
		targetVector.Z = 0.0f;
		
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
		
		m_OwnerAnimInstance->Montage_Play(m_NormalAttackMontage,1.0f);
		
		ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
	}
	else if (m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalAttack_OnGround) ||
		m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround))
	{
		if (ownerSkillComponent->HasStartedComboKeyInputCheck()) // 섹션점프 구간이면,
		{
			FVector targetVector = m_Owner->GetActorForwardVector() * m_MoveDistance;
			targetVector.Z = 0.0f;
			
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
			
			ownerSkillComponent->SetHasStartedComboKeyInputCheck(false);
			
			linqNextNormalAttackOnGroundCombo(); // 섹션점프
		}
	}
}

void UNormalAttack_OnGround::linqNextNormalAttackOnGroundCombo()
{
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	check(ownerSkillComponent != nullptr);
	
	if (m_CurComboAttackSection % 2 != 0) // 기본공격중인경우,
	{
		if (ownerSkillComponent->IsStrikeAttackActive()) // 강공격키 눌려있으면,
		{
			ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround);
			m_CurComboAttackSection += 1;
		}
		else  // 다음 기본공격 연계
		{
			ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
			m_CurComboAttackSection += 2;
		}
	}
	else // 강공격중인경우,
	{
		if (ownerSkillComponent->IsStrikeAttackActive())
		{
			m_CurComboAttackSection = FMath::Clamp(m_CurComboAttackSection + 2, 1, m_MaxNormalAttackSection);
	
			if (m_CurComboAttackSection == 7) // 마지막 공격
			{
				ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
			}
			else
			{
				ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround);
			}
		}
		else
		{
			ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_OnGround);
			m_CurComboAttackSection += 1;
		}
	}
	
	m_OwnerAnimInstance->JumpToMontageSectionByIndex(m_NormalAttackMontage, m_CurComboAttackSection);
}

bool UNormalAttack_OnGround::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() &&
		!m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}