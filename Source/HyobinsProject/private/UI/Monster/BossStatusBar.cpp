// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Monster/BossStatusBar.h"
#include "Component/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Materials/MaterialInstanceDynamic.h"

void UBossStatusBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));
	m_HPProgressBarBackground = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBarBackground")));
	
	m_StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_StaminaProgressBar")));
	m_StaminaProgressBarBackground = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_StaminaProgressBarBackground")));
	
	UpdateHPBar(1.0f);
	UpdateStaminaBar(1.0f);

	UObject* object = m_HPProgressBarBackground->WidgetStyle.BackgroundImage.GetResourceObject();
	UMaterialInterface* material = Cast<UMaterialInterface>(object);
	
	if (material != nullptr)
	{
		m_DynamicMaterial = UMaterialInstanceDynamic::Create(material, this);
		m_HPProgressBarBackground->WidgetStyle.BackgroundImage.SetResourceObject(m_DynamicMaterial);
	}
        
}

void UBossStatusBar::BindStatComponent(UStatComponent* statComponent)
{
	m_StatComponent = statComponent;
	m_StatComponent->OnChangedHP.AddUObject(this, &UBossStatusBar::UpdateHPBar);
	m_StatComponent->OnChangedStamina.AddUObject(this, &UBossStatusBar::UpdateStaminaBar);
}

void UBossStatusBar::UpdateHPBar(float changeAmount)
{
	if (m_StatComponent.IsValid())
	{
		m_HPProgressBar->SetPercent(m_StatComponent->GetHPRatio());

		if (m_DynamicMaterial != nullptr)
		{
			m_DynamicMaterial->SetScalarParameterValue(TEXT("UVStart"), m_HPProgressBar->Percent);
		}
		
		if (!GetWorld()->GetTimerManager().IsTimerActive(m_HPBarTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(
				m_HPBarTimer,
				this,&UBossStatusBar::UpdateHPBarBackground,
				GetWorld()->DeltaTimeSeconds,
				true,
				m_DecreaseSpeedFirstDelay
			);
		}
		
	}
}

void UBossStatusBar::UpdateHPBarBackground()
{
	float updatedPercent = m_HPProgressBarBackground->Percent - m_DecreaseSpeed;

	if (updatedPercent <= m_HPProgressBar->Percent)
	{
		m_HPProgressBarBackground->SetPercent(m_HPProgressBar->Percent);
		GetWorld()->GetTimerManager().ClearTimer(m_HPBarTimer);
	}

	m_HPProgressBarBackground->SetPercent(updatedPercent);
}

void UBossStatusBar::updateHPBarBackgroundColor(float uvStart, float uvEnd) // 
{
	
}


void UBossStatusBar::UpdateStaminaBar(float changeAmount)
{
	if (m_StatComponent.IsValid())
	{
		m_StaminaProgressBar->SetPercent(m_StatComponent->GetStaminaRatio());

		if (!GetWorld()->GetTimerManager().IsTimerActive(m_StaminaBarTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(
				m_StaminaBarTimer,
				this, &UBossStatusBar::UpdateStaminaBarBackground,
				GetWorld()->DeltaTimeSeconds,
				true,
				m_DecreaseSpeedFirstDelay
			);
		}
	}
}

void UBossStatusBar::UpdateStaminaBarBackground()
{
	float updatedPercent = m_StaminaProgressBarBackground->Percent - m_DecreaseSpeed;

	if (updatedPercent <= m_StaminaProgressBar->Percent)
	{
		m_StaminaProgressBarBackground->SetPercent(m_StaminaProgressBar->Percent);
		GetWorld()->GetTimerManager().ClearTimer(m_StaminaBarTimer);
	}

	m_StaminaProgressBarBackground->SetPercent(updatedPercent);
}

