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

	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_Owner->GetSkillComponent());
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("DashAttack_InAir"),
	[=]()
	{
		ownerSkillComponent->InitGravityScaleAfterAttack();
	});
}

void UDashAttack_InAir::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	if (!ownerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::EarthStrike_FallingToGround))
	{
		m_Owner->GetCharacterMovement()->GravityScale = ownerSkillComponent->GetGravityScaleInAir();
		m_Owner->RotateActorToKeyInputDirection();
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"),
			m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * m_MoveDistance);
		
		ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::DashAttack_InAir);
		
		m_OwnerAnimInstance->PlayMontage("DashAttack_InAir");
	}
}

bool UDashAttack_InAir::GetCanExecuteSkill() const
{
	return !m_Owner->GetIsCrowdControlState();
}
