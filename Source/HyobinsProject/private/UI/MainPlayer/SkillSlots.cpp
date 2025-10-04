// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayer/SkillSlots.h"
#include "Component/SkillComponent.h"
#include "UI/MainPlayer/SkillSlot.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "PlayableCharacter/Skill.h"

void USkillSlots::NativeConstruct()
{
	Super::NativeConstruct();

	m_BackGroundImage = Cast<UImage>(GetWidgetFromName(TEXT("m_BackGroundImage")));
	check(m_BackGroundImage != nullptr);
	
	m_HorizontalBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("m_HorizontalBox")));
	check(m_HorizontalBox != nullptr);
}

void USkillSlots::CreateSkillList(USkillComponent* skillComponent)
{
	m_SkillSlots.Empty();

	m_SkillComponent = skillComponent;
	check(m_SkillComponent.IsValid())
	
	const TMap<FName, FSkillList>* skillLists = m_SkillComponent->GetSkillList();
	UClass* skillSlotClass = LoadClass<UUserWidget>(
		nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/SkillSlot.SkillSlot_C'"));
	check(skillSlotClass != nullptr);

	bool bIsFirst = true;

	for (const auto skillList : *skillLists)
	{
		const FName inputMappingContextName = skillList.Key;

		if (bIsFirst)
		{
			m_CurModeName = inputMappingContextName;
		}

		FSkillSlotInfos skillSlots;

		for (const auto skillInfo : skillList.Value.skillList)
		{
			USkill* skill = skillInfo.Value;

			if (IsValid(skill) && skill->GetIsAddedSkillSlots())
			{
				USkillSlot* skillSlot = Cast<USkillSlot>(CreateWidget(GetWorld(), skillSlotClass));

				const FMargin paddingValue = {m_LeftPadding, m_TopPadding, m_RightPadding, m_BottomPadding};
				skillSlot->SetSkill(skill, paddingValue);
				skillSlots.skillSlots.Add(skillSlot);

				if (bIsFirst) // 컨텍스트마다 한번씩만 
				{
					m_HorizontalBox->AddChildToHorizontalBox(skillSlot);
				}
			}
		}

		bIsFirst = false;

		m_SkillSlots.Add(inputMappingContextName, skillSlots);
	}
	
}

void USkillSlots::ChangeSkillSlots()
{
	FName highestPriorityModeName = m_SkillComponent->GetHighestPriorityInputMappingContext();

	if (highestPriorityModeName == TEXT("None"))
	{
		return;
	}
	
	if (m_CurModeName != highestPriorityModeName)
	{
		for (TObjectPtr<USkillSlot> skillSlot : m_SkillSlots[m_CurModeName].skillSlots) // 현재(바뀌기 전) 컨텍스트 각 스킬슬롯 삭제.
		{
			skillSlot->RemoveFromParent();
		}
	
		for (TObjectPtr<USkillSlot> skillSlot : m_SkillSlots[highestPriorityModeName].skillSlots) // 바뀔 컨텍스트 스킬슬롯 추가.
		{
			m_HorizontalBox->AddChildToHorizontalBox(skillSlot);
		}

		m_CurModeName = highestPriorityModeName;
	}
}


