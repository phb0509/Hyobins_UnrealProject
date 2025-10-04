// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Guard_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/EnumTypes.h"


UGuard_OnGround::UGuard_OnGround() :
	m_GuardMontage(nullptr),
	m_ParryingTime(0.5f)
{
}

void UGuard_OnGround::Initialize()
{
	Super::Initialize();
	
	check(m_GuardMontage != nullptr);
}

void UGuard_OnGround::Execute()
{
	Super::Execute();

	AMainPlayer* owner = Cast<AMainPlayer>(m_Owner);
	check(owner != nullptr);
	
	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::Parry_OnGround);
	m_OwnerAnimInstance->Montage_Play(m_GuardMontage, 1.0f);
	
	owner->GetCollider(TEXT("ShieldForGuardCollider"))->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	owner->SetIsParrying(true);
	
	owner->GetWorldTimerManager().SetTimer(
	   m_ParryingTimer,
	   [=]()
	   {
	   		owner->SetIsParrying(false);
	   		owner->GetCollider(TEXT("ShieldForGuardCollider"))->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	   },
	   m_ParryingTime,   
	   false   
   );
	
}

bool UGuard_OnGround::CanExecuteSkill() const
{
	if (m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::None))
	{
		return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState();
	}
	
	bool skillRequirements =
		m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalAttack_OnGround) ||
		m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround);
	
	return Super::CanExecuteSkill() && skillRequirements;
}
