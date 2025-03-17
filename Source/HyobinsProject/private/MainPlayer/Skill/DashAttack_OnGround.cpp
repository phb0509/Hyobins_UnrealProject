// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/DashAttack_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
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
    		m_Owner->SetIsSuperArmor(false);
    	});
}

void UDashAttack_OnGround::Execute()
{
	Super::Execute();

	if (!m_bIsCoolDownActive)
	{
		return;
	}
	
	m_bIsCoolDownActive = false;
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetSkillState();

	OnExecute.Broadcast();

	m_Owner->RotateActorToKeyInputDirection();
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"),
		m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
	
	m_OwnerSkillComponent->SetCanDodge(false);
	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::DashAttack_OnGround);
	
	m_OwnerAnimInstance->PlayMontage("DashAttack_OnGround");
}
