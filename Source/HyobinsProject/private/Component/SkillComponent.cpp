// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"
#include "MainPlayer/Skill/Skill.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 
	bWantsInitializeComponent = false;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	loadSkills();
}

void USkillComponent::ExecEvent_AllSkillEnded()
{
}

void USkillComponent::ExecuteSkill(const FName& inputMappingContextName, const FName& skillName)
{
	if (HasSkill(inputMappingContextName, skillName))
	{
		m_CurSkill = m_SkillList[inputMappingContextName].skillList[skillName];
		m_CurSkill->Execute();
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
			const FName skillName = skill->GetSkillName();

			m_SkillList[inputMappingContextName].skillList.Add(skillName, skill);
		}
	}
}
