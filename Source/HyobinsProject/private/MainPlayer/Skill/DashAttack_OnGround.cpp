// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/DashAttack_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UDashAttack_OnGround::UDashAttack_OnGround() :
	m_DashAttackMontage(nullptr),
	m_MoveDistance(500.0f)
{
}

void UDashAttack_OnGround::Initialize()
{
	Super::Initialize();

	check(m_DashAttackMontage != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("DashAttack_OnGround"),
    	[this]()
    	{
    		m_Owner->SetIsSuperArmor(false);
    	});
}

void UDashAttack_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	check(ownerSkillComponent != nullptr);
	
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"),
		m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
	
	ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::DashAttack_OnGround);
	
	m_OwnerAnimInstance->Montage_Play(m_DashAttackMontage, 1.0f);
}

bool UDashAttack_OnGround::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() &&
		!m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}
