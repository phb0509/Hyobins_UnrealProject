// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Combo.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"



void UCombo::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_ComboCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("m_ComboCountText")));
	check(m_ComboCountText != nullptr);
}

void UCombo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCombo::UpdateComboCount()
{
	++m_ComboCount;
	
	m_ComboCountText->SetText(FText::AsNumber(m_ComboCount));
	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	GetWorld()->GetTimerManager().ClearTimer(m_ComboCountTimer);
	GetWorld()->GetTimerManager().SetTimer
		(
			m_ComboCountTimer,
			[this]()
			{
				m_ComboCount = 0;
				this->SetVisibility(ESlateVisibility::Collapsed);
			},
		m_ComboCountDecisionTime,
		false);

	PlayAnimation(m_ComboAnimation);	
}
