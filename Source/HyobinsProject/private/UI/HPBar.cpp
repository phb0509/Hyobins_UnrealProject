// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/HPBar.h"
#include "Component/StatComponent.h"
#include "Components/ProgressBar.h"

void UHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));
	updateHPWidget();
}

void UHPBar::BindStatComponent(UStatComponent* statComponent)
{
	m_StatComponent = statComponent;
	m_StatComponent->OnHPIsChanged.AddUObject(this, &UHPBar::updateHPWidget);
}

void UHPBar::SetExecuteState()
{
	m_HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UHPBar::updateHPWidget()
{
	if (m_StatComponent->IsValidLowLevel())
	{
		if (m_HPProgressBar != nullptr)
		{
			m_HPProgressBar->SetPercent(m_StatComponent->GetHPRatio());
		}
	}
}
