// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayer/MainPlayerStatusBar.h"
#include "Component/StatComponent.h"
#include "Components/ProgressBar.h"

void UMainPlayerStatusBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));
	m_StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_StaminaProgressBar")));
	
	UpdateHPBar();
	UpdateStaminaBar();
}

void UMainPlayerStatusBar::BindStatComponent(UStatComponent* statComponent)
{
	m_OwnerStatComponent = statComponent;
	m_OwnerStatComponent->OnDamagedHP.AddUObject(this, &UMainPlayerStatusBar::UpdateHPBar);
	m_OwnerStatComponent->OnRecoveredHP.AddUObject(this, &UMainPlayerStatusBar::UpdateHPBar);
	
	m_OwnerStatComponent->OnDamagedStamina.AddUObject(this, &UMainPlayerStatusBar::UpdateStaminaBar);
	m_OwnerStatComponent->OnRecoveredStamina.AddUObject(this, &UMainPlayerStatusBar::UpdateStaminaBar);
}

void UMainPlayerStatusBar::UpdateHPBar()
{
	if (m_OwnerStatComponent.IsValid())
	{
		m_HPProgressBar->SetPercent(m_OwnerStatComponent->GetHPRatio());
	}
}

void UMainPlayerStatusBar::UpdateStaminaBar()
{
	if (m_OwnerStatComponent.IsValid())
	{
		m_StaminaProgressBar->SetPercent(m_OwnerStatComponent->GetStaminaRatio());
	}
}
