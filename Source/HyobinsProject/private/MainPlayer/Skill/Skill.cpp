// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Skill.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"

USkill::USkill() :
	m_CoolDownTime(3.0f),
	m_bIsCoolDownActive(true),
	m_StaminaCost(10.0f),
	m_bIsSuperArmor(false),
	m_ThumbnailFillTexture(nullptr),
	m_ThumbnailBackgroundTexture(nullptr)
{
}

void USkill::Execute()
{
	if (!m_bIsCoolDownActive)
	{
		return;
	}

	m_Owner->SetIsSuperArmor(m_bIsSuperArmor);
	FTimerHandle timer;
	m_Owner->GetWorldTimerManager().SetTimer(timer,
					[this]()
					{
						m_bIsCoolDownActive = true;
					},
				m_CoolDownTime,
				false);
}

void USkill::SetOwnerInfo(AMainPlayer* owner)
{
	m_Owner = owner;

	if (m_Owner.IsValid())
	{
		m_OwnerAnimInstance = Cast<UMainPlayerAnim>(m_Owner->GetMesh()->GetAnimInstance());
		m_OwnerSkillComponent = m_Owner->GetSkillComponent();
	}
}
