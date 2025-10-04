// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Skill.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/SkillComponent.h"
#include "Component/StatComponent.h"

USkill::USkill() :
	m_bIsAddedSkillSlots(true),
	m_CoolDownTime(3.0f),
	m_bIsCooldownComplete(true),
	m_StaminaCost(10.0f),
	m_bIsSuperArmor(false),
	m_ThumbnailFillTexture(nullptr),
	m_ThumbnailBackgroundTexture(nullptr)
{
}

void USkill::Execute()
{
	m_bIsCooldownComplete = false;
	
	m_Owner->GetWorldTimerManager().SetTimer(
		m_TimerHandle,
		[this]()
		{
			m_bIsCooldownComplete = true;
		},
		m_CoolDownTime,
		false);

	m_Owner->SetIsSuperArmor(m_bIsSuperArmor);
	m_Owner->GetStatComponent()->OnDamageStamina(m_StaminaCost * 0.5f);
	
	if (m_Owner->IsLockOnMode())
	{
		AActor* lockOnTarget = m_Owner->GetCurLockOnTarget();

		if (lockOnTarget != nullptr)
		{
			m_Owner->RotateToTarget(lockOnTarget);
		}
	}
	else
	{
		m_Owner->RotateActorToKeyInputDirection();
	}

	if (m_bIsAddedSkillSlots)
	{
		OnExecute.Broadcast(); // 스킬아이콘 쿨다운 업데이트.
	}
}

bool USkill::CanExecuteSkill() const
{
	return m_bIsCooldownComplete && m_Owner->HasEnoughStamina(m_StaminaCost);
}

void USkill::SetOwnerInfo(APlayableCharacter* owner)
{
	m_Owner = owner;
	check(m_Owner.IsValid())
	
	m_OwnerAnimInstance = Cast<UAnimInstanceBase>(m_Owner->GetMesh()->GetAnimInstance());
	check(m_OwnerAnimInstance.IsValid());
	
	m_OwnerSkillComponent = m_Owner->GetSkillComponent();
	check(m_OwnerSkillComponent.IsValid());
}



