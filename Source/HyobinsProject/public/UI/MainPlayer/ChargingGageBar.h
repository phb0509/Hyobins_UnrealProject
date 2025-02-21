// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "ChargingGageBar.generated.h"


class UProgressBar;
class UTextBlock;

UCLASS()
class HYOBINSPROJECT_API UChargingGageBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void StartCharging(float chargingDuration);
	void LoopCharging();
	void SetWidgetComponent(UWidgetComponent* widgetComponent) { m_WidgetComponent = widgetComponent; }

	UFUNCTION()
	void Remove();



private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> m_ChargingGageBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> m_CompleteText;

	UPROPERTY(Transient, meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> m_CompleteTextAnimation;

	TWeakObjectPtr<UWidgetComponent> m_WidgetComponent;

	FTimerHandle m_ChargingBarUpdateTimer;
	float m_CurAccumulatedTime;
	float m_CurUpdateDuration;
};
