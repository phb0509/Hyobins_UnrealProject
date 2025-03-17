// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Monster/HPBar.h"
#include "Component/StatComponent.h"
#include "Components/ProgressBar.h"

void UHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));
	UpdateHPBar(1.0f); 
}

void UHPBar::BindStatComponent(UStatComponent* statComponent)
{
	m_StatComponent = statComponent;
	m_StatComponent->OnChangedHP.AddUObject(this, &UHPBar::UpdateHPBar);
}

void UHPBar::UpdateHPBar(float changeAmount) // ���������� ���ط�.
{
	// �� Ÿ��ü�¹� ����

	if (m_StatComponent.IsValid())
	{
		m_HPProgressBar->SetPercent(m_StatComponent->GetHPRatio());
	}
}
