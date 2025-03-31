// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/SkillComponent.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "MainPlayer/Skill/Skill.h"
#include "SubSystems/UIManager.h"

USkillComponent::USkillComponent() :
	m_CurSkillState(0)
{
	PrimaryComponentTick.bCanEverTick = false; 
	bWantsInitializeComponent = false;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = Cast<APlayableCharacter>(GetOwner());
	m_OwnerAnimInstance = Cast<UAnimInstanceBase>(m_Owner->GetMesh()->GetAnimInstance());
	
	loadSkills();
	initSkills();
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateSkillSlots(this, m_Owner.Get());
}

FName USkillComponent::GetHighestPriorityInputMappingContext() const
{
	return m_Owner->GetHighestPriorityInputMappingContext();
}

void USkillComponent::ExecuteSkill(const FName& inputMappingContextName, const FName& skillName)
{
	if (HasSkill(inputMappingContextName, skillName))
	{
		m_CurSkill = m_SkillList[inputMappingContextName].skillList[skillName];

		if (m_CurSkill->GetCanUseSkill())
		{
			m_CurSkill->Execute();
		}
	}
}

void USkillComponent::loadSkills()
{
	for (const auto iter : m_SkillClassList)
	{
		const FName inputMappingContextName = iter.Key;

		FSkillList skillList;
		m_SkillList.Add(inputMappingContextName, skillList);
		
		for (const TSubclassOf<USkill> skillClass : iter.Value.skillClassList)
		{
			USkill* skill = Cast<USkill>(NewObject<UObject>(GetTransientPackage(), skillClass));
			const FName skillName = skill->GetName();

			m_SkillList[inputMappingContextName].skillList.Add(skillName, skill);
		}
	}
}

void USkillComponent::initSkills()
{
	for (const auto iter : m_SkillList)
	{
		for (const auto skill : iter.Value.skillList)
		{
			if (skill.Value != nullptr)
			{
				skill.Value->SetOwnerInfo(m_Owner.Get());
				skill.Value->Initialize();
			}
		}
	}
}
