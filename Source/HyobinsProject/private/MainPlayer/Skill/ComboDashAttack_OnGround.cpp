// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/ComboDashAttack_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/EnumTypes.h"

UComboDashAttack_OnGround::UComboDashAttack_OnGround()
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
		m_Owner->SetIsSuperArmor(false, false);
	});
}

void UComboDashAttack_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	if (ownerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround) &&
		ownerSkillComponent->CanChargingSkill())
	{
		ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::Charging_ComboDashAttack_OnGround);
		
		m_OwnerAnimInstance->PlayMontage(TEXT("Charging_ComboDashAttack_OnGround"));
	}
}

bool UComboDashAttack_OnGround::CanExecuteSkill() const
{
	return !m_Owner->IsCrowdControlState();
}
