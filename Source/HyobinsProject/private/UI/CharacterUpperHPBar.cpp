// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterUpperHPBar.h"
#include "Component/StatComponent.h"
#include "Components/ProgressBar.h"

void UCharacterUpperHPBar::BindStatComponent(UStatComponent* statComponent)
{
	m_StatComponent = statComponent;
	m_StatComponent->OnHPIsChanged.AddUObject(this, &UCharacterUpperHPBar::UpdateHPWidget);
}

void UCharacterUpperHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UCharacterUpperHPBar::UpdateHPWidget()
{
	if (m_StatComponent->IsValidLowLevel())
	{
		if (m_HPProgressBar != nullptr)
		{
			m_HPProgressBar->SetPercent(m_StatComponent->GetHPRatio());
		}
	}
}
