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
	
	AMainPlayer* owner = Cast<AMainPlayer>(m_Owner);
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Charging_ComboDashAttack_OnGround"),
	[=]()
	{
		ownerSkillComponent->SetCanChargingSkill(false);
		owner->RemoveInputContextMappingOnCharging();
		owner->SetIsSuperArmor(false);
	});
}

void UComboDashAttack_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	if (ownerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround) &&
		ownerSkillComponent->GetCanChargingSkill())
	{
		ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::Charging_ComboDashAttack_OnGround);
		
		m_OwnerAnimInstance->PlayMontage(TEXT("Charging_ComboDashAttack_OnGround"));
	}
}

bool UComboDashAttack_OnGround::GetCanExecuteSkill() const
{
	return !m_Owner->GetIsCrowdControlState();
}
