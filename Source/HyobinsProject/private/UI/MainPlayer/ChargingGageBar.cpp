// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayer/ChargingGageBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UChargingGageBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_ChargingGageBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_ChargingGageBar")));
	m_CompleteText = Cast<UTextBlock>(GetWidgetFromName(TEXT("m_CompleteText")));
	m_CompleteText->SetVisibility(ESlateVisibility::Collapsed);

	FWidgetAnimationDynamicEvent endDelegate;
	endDelegate.BindDynamic(this, &UChargingGageBar::Remove);
	BindToAnimationFinished(m_CompleteTextAnimation, endDelegate);
}

void UChargingGageBar::StartCharging(float chargingDuration)
{
	m_CurUpdateDuration = chargingDuration;
	m_CurAccumulatedTime = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(
            m_ChargingBarUpdateTimer, 
            this, 
            &UChargingGageBar::LoopCharging, 
            GetWorld()->DeltaTimeSeconds,  // 호출 간격
            true     // 반복 여부
        );
}

void UChargingGageBar::LoopCharging()
{
	m_CurAccumulatedTime += GetWorld()->DeltaTimeSeconds;
	m_ChargingGageBar->SetPercent(m_CurAccumulatedTime / m_CurUpdateDuration);
	
	if (m_CurAccumulatedTime >= m_CurUpdateDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_ChargingBarUpdateTimer);

		m_CompleteText->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayAnimation(m_CompleteTextAnimation);
	}
}


void UChargingGageBar::Remove()
{
	if (m_WidgetComponent.IsValid())
	{
		m_WidgetComponent->DestroyComponent();
	}
}
