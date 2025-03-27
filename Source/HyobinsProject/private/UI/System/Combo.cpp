// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Combo.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "CharacterBase/CharacterBase.h"


void UCombo::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_ComboCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("m_ComboCountText")));
}

void UCombo::BindActor(ACharacterBase* takeDamageActor)
{
	if (IsValid(takeDamageActor))
	{
		takeDamageActor->OnTakeDamage.AddUObject(this, &UCombo::UpdateComboCount);
	}
}

void UCombo::UpdateComboCount(const FHitInformation& hitInfo)
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
