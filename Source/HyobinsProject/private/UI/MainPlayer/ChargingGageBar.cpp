// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayer/ChargingGageBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UChargingGageBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_ChargingGageBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_ChargingGageBar")));
	check(m_ChargingGageBar != nullptr);
	
	m_CompleteText = Cast<UTextBlock>(GetWidgetFromName(TEXT("m_CompleteText")));
	check(m_CompleteText != nullptr);
	
	m_CompleteText->SetVisibility(ESlateVisibility::Collapsed);

	FWidgetAnimationDynamicEvent endDelegate;
	endDelegate.BindDynamic(this, &UChargingGageBar::Remove);
	
	BindToAnimationFinished(m_CompleteTextAnimation, endDelegate);
}

void UChargingGageBar::NativeDestruct()
{
	Super::NativeDestruct();
	
	UnbindAllFromAnimationFinished(m_CompleteTextAnimation);
}

void UChargingGageBar::StartCharging(float chargingDuration)
{
	m_CurUpdateDuration = chargingDuration;
	m_CurAccumulatedTime = 0.0f;
	m_TimerInterval = 1.0f / m_UpdateFPS;

	GetWorld()->GetTimerManager().SetTimer(
		m_ChargingBarUpdateTimer, 
		this, 
		&UChargingGageBar::LoopCharging, 
		m_TimerInterval,  
		true            
	);
}

void UChargingGageBar::LoopCharging()
{
	m_CurAccumulatedTime += m_TimerInterval;

	float percent = FMath::Clamp(m_CurAccumulatedTime / m_CurUpdateDuration, 0.0f, 1.0f);
	m_ChargingGageBar->SetPercent(percent);

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
