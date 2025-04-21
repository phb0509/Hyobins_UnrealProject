// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/NormalAttack_InAir.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UNormalAttack_InAir::UNormalAttack_InAir():
	m_CurComboAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_MoveDistance(100.0f)
{
}

void UNormalAttack_InAir::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageStarted(TEXT("NormalAttack_InAir"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = m_OwnerSkillComponent->GetGravityScaleInAir();
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::NormalAttack_InAir);
	});
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("NormalAttack_InAir"),
	[this]()
	{
		m_OwnerSkillComponent->InitGravityScaleAfterAttack();
	});
	
		m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack_InAir"),
	[this]()
		{
			m_CurComboAttackSection = 1;
		});
}

void UNormalAttack_InAir::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	if (ownerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::NormalAttack_InAir))
	{
		if (ownerSkillComponent->HasStartedComboKeyInputCheck()) // 섹션점프 구간이면,
		{
			m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.
			
			FVector targetVector = m_Owner->GetActorForwardVector() * m_MoveDistance;
			targetVector.Z = 0.0f;
			m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);
			
			ownerSkillComponent->SetHasStartedComboKeyInputCheck(false);
			
			linqNextNormalAttackInAirCombo(); // 섹션점프
		}
	}
	else if (ownerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Idle))
	{
		m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.

		FVector targetVector = m_Owner->GetActorForwardVector() * m_MoveDistance;
		targetVector.Z = 0.0f;
		m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("Forward"), m_Owner->GetActorLocation() + targetVector);

		m_OwnerAnimInstance->PlayMontage(TEXT("NormalAttack_InAir"));
	}
}

void UNormalAttack_InAir::linqNextNormalAttackInAirCombo()
{
	m_CurComboAttackSection += 1;
	m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("NormalAttack_InAir"), m_CurComboAttackSection);
}

bool UNormalAttack_InAir::GetCanExecuteSkill() const
{
	return !m_Owner->IsCrowdControlState();
}