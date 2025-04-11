// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Guard_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Interfaces/Damageable.h"
#include "Utility/EnumTypes.h"
#include "Kismet/GameplayStatics.h"

UGuard_OnGround::UGuard_OnGround()
{
}

void UGuard_OnGround::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("GuardEnd_OnGround"),
	[this]()
	{
		m_Owner->SetIsSuperArmor(false);
	});
}

void UGuard_OnGround::Execute()
{
	Super::Execute();

	AMainPlayer* owner = Cast<AMainPlayer>(m_Owner);

	if (!owner->IsGuard())
	{
		owner->SetIsGuard(true);
		m_OwnerAnimInstance->PlayMontage("Guard_OnGround");
	}
	else
	{
		owner->SetIsGuard(false);
		m_OwnerAnimInstance->PlayMontage("GuardEnd_OnGround");
	}
}

bool UGuard_OnGround::GetCanExecuteSkill() const
{
	return !m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}
