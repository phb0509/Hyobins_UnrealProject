// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Monster/BossStatusBar.h"
#include "Component/StatComponent.h"
#include "Components/ProgressBar.h"

void UBossStatusBar::NativeConstruct()
{
	Super::NativeConstruct();
	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));
	m_StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_StaminaProgressBar")));
	
	UpdateHPBar();
	UpdateStaminaBar();
}

void UBossStatusBar::BindStatComponent(UStatComponent* statComponent)
{
	m_StatComponent = statComponent;
	m_StatComponent->OnChangedHP.AddUObject(this, &UBossStatusBar::UpdateHPBar);
	m_StatComponent->OnChangedStamina.AddUObject(this, &UBossStatusBar::UpdateStaminaBar);
}

void UBossStatusBar::UpdateHPBar()
{
	m_HPProgressBar->SetPercent(m_StatComponent->GetHPRatio());
}

void UBossStatusBar::UpdateStaminaBar()
{
	m_StaminaProgressBar->SetPercent(m_StatComponent->GetStaminaRatio());
}
