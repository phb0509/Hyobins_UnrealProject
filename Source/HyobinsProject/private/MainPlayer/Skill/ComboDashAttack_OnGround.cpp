// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/ComboDashAttack_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/EnumTypes.h"

UComboDashAttack_OnGround::UComboDashAttack_OnGround()
{
}

void UComboDashAttack_OnGround::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Charging_ComboDashAttack_OnGround"),
	[this]()
	{
		m_OwnerSkillComponent->SetCanChargingSkill(false);
		m_Owner->RemoveInputContextMappingOnCharging();
		m_Owner->SetIsSuperArmor(false);
	});
}

void UComboDashAttack_OnGround::Execute()
{
	Super::Execute();
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetSkillState();
	
	if (curSkillState == EMainPlayerSkillStates::Charging_OnGround && m_OwnerSkillComponent->GetCanChargingSkill())
	{
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::Charging_ComboDashAttack_OnGround);
		
		m_OwnerAnimInstance->PlayMontage(TEXT("Charging_ComboDashAttack_OnGround"));
	}
}

bool UComboDashAttack_OnGround::GetCanExecuteSkill() const
{
	return !m_Owner->GetIsCrowdControlState();
}
