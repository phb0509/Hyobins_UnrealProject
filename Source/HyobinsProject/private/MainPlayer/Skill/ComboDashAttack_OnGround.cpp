// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/ComboDashAttack_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/EnumTypes.h"

UComboDashAttack_OnGround::UComboDashAttack_OnGround() :
	m_ComboDashAttackMontage(nullptr)
{
}

void UComboDashAttack_OnGround::Initialize()
{
	Super::Initialize();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Charging_ComboDashAttack_OnGround"),
	[=]()
	{
		ownerSkillComponent->SetCanChargingSkill(false);
		m_Owner->RemoveInputMappingContext(TEXT("Charging_OnGround"));
		m_Owner->AddInputMappingContext(TEXT("Default_OnGround")); 
		m_Owner->SetIsSuperArmor(false);
	});
}

void UComboDashAttack_OnGround::Execute()
{
	Super::Execute();
	
	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::Charging_ComboDashAttack_OnGround);
	
	m_OwnerAnimInstance->Montage_Play(m_ComboDashAttackMontage,1.0f);
}

bool UComboDashAttack_OnGround::CanExecuteSkill() const
{
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState() &&
		ownerSkillComponent->CanChargingSkill();
}
