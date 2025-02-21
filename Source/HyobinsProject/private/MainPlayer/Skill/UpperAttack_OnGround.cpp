// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/UpperAttack_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UUpperAttack_OnGround::UUpperAttack_OnGround():
	m_MoveDistance(140.0f),
	m_JumpDistance(500.0f)
{
}

void UUpperAttack_OnGround::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("UpperAttack_GroundToAir"),
	[this]()
	{
		m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	});
}

void UUpperAttack_OnGround::Execute()
{
	Super::Execute();

	if (!m_bIsCoolDownActive)
	{
		return;
	}
	
	m_bIsCoolDownActive = false;
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetState();
	
	if (curSkillState == EMainPlayerSkillStates::Idle ||
		curSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||
		curSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
	{
		OnExecute.Broadcast();
		
		m_Owner->RotateActorToKeyInputDirection();

		if (m_OwnerSkillComponent->GetIsStrikeAttackActive())
		{
			m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::UpperAttack_GroundToAir);
			m_OwnerAnimInstance->PlayMontage(TEXT("UpperAttack_GroundToAir"));
			m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying); // Flying모드로 해야 모션워핑이 z축이동.

			const FVector targetLocation = m_Owner->GetActorLocation() +
				(m_Owner->GetActorUpVector() * m_JumpDistance) +
				(m_Owner->GetActorForwardVector() * m_MoveDistance);
			
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), targetLocation);
		}
		else
		{
			m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::UpperAttack_OnGround);
			m_OwnerAnimInstance->PlayMontage("UpperAttack_OnGround");

			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"),
				m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
		}
	}
}
