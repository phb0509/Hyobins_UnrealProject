// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/SkillComponent.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "PlayableCharacter/Skill.h"
#include "SubSystems/UIManager.h"

USkillComponent::USkillComponent() :
	m_CurSkillState(0),
	m_GravityScaleInAir(0.00001f)
{
	PrimaryComponentTick.bCanEverTick = false; 
	bWantsInitializeComponent = false;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = Cast<APlayableCharacter>(GetOwner());
	check(m_Owner != nullptr);
	
	m_OwnerAnimInstance = Cast<UAnimInstanceBase>(m_Owner->GetMesh()->GetAnimInstance());
	check(m_OwnerAnimInstance != nullptr);
	
	loadSkills();
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateSkillSlots(this, m_Owner.Get());
}

void USkillComponent::ExecuteSkill(const FName& inputMappingContextName, const FName& skillName)
{
	if (HasSkill(inputMappingContextName, skillName))
	{
		m_CurSkill = m_SkillList[inputMappingContextName].skillList[skillName];

		if (m_CurSkill->CanExecuteSkill())
		{
			m_CurSkill->Execute();
		}
	}
}

void USkillComponent::InitGravityScaleAfterAttack()
{
	if (m_Owner->GetCharacterMovement()->GravityScale == m_GravityScaleInAir)
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	}
}

bool USkillComponent::HasSkill(const FName& inputMappingContextName, const FName& skillName) const
{
	return m_SkillList.Contains(inputMappingContextName) &&
			m_SkillList[inputMappingContextName].skillList.Contains(skillName) &&
			m_SkillList[inputMappingContextName].skillList[skillName] != nullptr;
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
			skill->SetOwnerInfo(m_Owner.Get());
			skill->Initialize();
			
			const FName skillName = skill->GetName();
			m_SkillList[inputMappingContextName].skillList.Add(skillName, skill);
		}
	}
}


