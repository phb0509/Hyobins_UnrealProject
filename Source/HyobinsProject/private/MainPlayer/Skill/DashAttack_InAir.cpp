// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/DashAttack_InAir.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UDashAttack_InAir::UDashAttack_InAir():
	m_MoveDistance(150.0f)
{
}

void UDashAttack_InAir::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("DashAttack_InAir"),
	[this]()
	{
		m_OwnerSkillComponent->InitGravityScaleAfterAttack();
	});
}

void UDashAttack_InAir::Execute()
{
	Super::Execute();

	if (!m_bIsCoolDownActive)
	{
		return;
	}
	
	m_bIsCoolDownActive = false;
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetState();
	
	if (curSkillState != EMainPlayerSkillStates::EarthStrike_FallingToGround)
	{
		OnExecute.Broadcast();
		
		m_Owner->GetCharacterMovement()->GravityScale = m_OwnerSkillComponent->GetGravityScaleInAir();
		
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::DashAttack_InAir);
		m_Owner->RotateActorToKeyInputDirection();
		m_OwnerAnimInstance->PlayMontage("DashAttack_InAir");
	
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"),
			m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
	}
}
