// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayer/SkillSlots.h"
#include "Component/SkillComponent.h"
#include "UI/MainPlayer/SkillSlot.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void USkillSlots::NativeConstruct()
{
	Super::NativeConstruct();

	m_BackGroundImage = Cast<UImage>(GetWidgetFromName(TEXT("m_BackGroundImage")));
	m_HorizontalBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("m_HorizontalBox")));
}

void USkillSlots::CreateSkillListFromSkillComponent(USkillComponent* skillComponent)
{
	m_SkillSlots.Empty();

	if (IsValid(skillComponent))
	{
		m_SkillComponent = skillComponent;
	
		const TMap<FName, FSkillList>* skillList = m_SkillComponent->GetSkillList();
		UClass* skillSlotClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/SkillSlot.SkillSlot_C'"));

		bool bIsFirst = true;
	
		for (const auto iter : *skillList)
		{
			const FName inputMappingContextName = iter.Key;
			
			if (bIsFirst)
			{
				m_CurModeName = inputMappingContextName;
			}
		
			FSkillSlotInfos skillSlots;
		
			for (const auto skill : iter.Value.skillList)
			{
				FName skillName = skill.Key;
				USkill* skillType = skill.Value;

				if (skillType != nullptr)
				{
					USkillSlot* skillSlot = Cast<USkillSlot>(CreateWidget(GetWorld(), skillSlotClass));

					const FMargin paddingValue = {m_LeftPadding, m_TopPadding, m_RightPadding, m_BottomPadding};
					skillSlot->SetSkill(skillType, paddingValue);
					skillSlots.skillSlots.Add(skillSlot);

					if (bIsFirst)
					{
						m_HorizontalBox->AddChildToHorizontalBox(skillSlot);
					}
				}
			}
		
			bIsFirst = false;
			m_SkillSlots.Add(inputMappingContextName,skillSlots);
		}
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
		for (TObjectPtr<USkillSlot> skillSlot : m_SkillSlots[m_CurModeName].skillSlots)
		{
			skillSlot->RemoveFromParent();
		}
	
		for (TObjectPtr<USkillSlot> skillSlot : m_SkillSlots[highestPriorityModeName].skillSlots)
		{
			m_HorizontalBox->AddChildToHorizontalBox(skillSlot);
		}

		m_CurModeName = highestPriorityModeName;
	}
}


