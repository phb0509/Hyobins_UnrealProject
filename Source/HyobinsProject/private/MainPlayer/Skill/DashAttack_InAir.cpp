// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/DashAttack_InAir.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/SkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UDashAttack_InAir::UDashAttack_InAir():
	m_DashAttackMontage(nullptr),
	m_MoveDistance(150.0f)
{
}

void UDashAttack_InAir::Initialize()
{
	Super::Initialize();

	check(m_DashAttackMontage != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("DashAttack_InAir"),
	[this]()
	{
		m_OwnerSkillComponent->InitGravityScaleAfterAttack();
	});
}

void UDashAttack_InAir::Execute()
{
	Super::Execute();
	
	m_Owner->GetCharacterMovement()->GravityScale = m_OwnerSkillComponent->GetGravityScaleInAir();
	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"),
		m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);

	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::DashAttack_InAir);

	m_OwnerAnimInstance->Montage_Play(m_DashAttackMontage, 1.0f);
}

bool UDashAttack_InAir::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() &&
		!m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::EarthStrike_FallingToGround);
}
