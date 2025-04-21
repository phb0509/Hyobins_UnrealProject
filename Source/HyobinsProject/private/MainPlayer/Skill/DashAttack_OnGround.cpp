// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/DashAttack_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UDashAttack_OnGround::UDashAttack_OnGround() :
	m_MoveDistance(500.0f)
{
}

void UDashAttack_OnGround::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("DashAttack_OnGround"),
    	[this]()
    	{
    		m_Owner->SetIsSuperArmor(false, false);
    	});
}

void UDashAttack_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	m_Owner->RotateActorToKeyInputDirection();
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"),
		m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
	
	ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::DashAttack_OnGround);
	
	m_OwnerAnimInstance->PlayMontage("DashAttack_OnGround");
}

bool UDashAttack_OnGround::GetCanExecuteSkill() const
{
	return !m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}
