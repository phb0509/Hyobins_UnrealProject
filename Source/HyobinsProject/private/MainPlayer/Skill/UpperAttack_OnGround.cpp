// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/UpperAttack_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UUpperAttack_OnGround::UUpperAttack_OnGround():
	m_UpperAttackMontage(nullptr),
	m_UpperAttackToAirMontage(nullptr),
	m_MoveDistance(140.0f),
	m_JumpDistance(500.0f)
{
}

void UUpperAttack_OnGround::Initialize()
{
	Super::Initialize();

	check(m_UpperAttackMontage != nullptr);
	check(m_UpperAttackToAirMontage != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("UpperAttack_GroundToAir"),
	[this]()
	{
		m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	});
}

void UUpperAttack_OnGround::Execute()
{
	Super::Execute();
	
	if (m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::None) ||
		m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalAttack_OnGround) ||
		m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalStrikeAttack_OnGround))
	{
		UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
		check(ownerSkillComponent != nullptr);
		
		if (ownerSkillComponent->IsStrikeAttackActive())
		{
			m_Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying); // Flying모드로 해야 모션워핑이 z축이동.

			const FVector targetLocation = m_Owner->GetActorLocation() +
				(m_Owner->GetActorUpVector() * m_JumpDistance) +
				(m_Owner->GetActorForwardVector() * m_MoveDistance);
			
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), targetLocation);
			
			ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::UpperAttack_GroundToAir);
			
			m_OwnerAnimInstance->Montage_Play(m_UpperAttackToAirMontage, 1.0f);
		}
		else
		{
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"),
				m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
			
			ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::UpperAttack_OnGround);
			
			m_OwnerAnimInstance->Montage_Play(m_UpperAttackMontage, 1.0f);
		}
	}
}

bool UUpperAttack_OnGround::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() &&
		!m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}
