// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossStatusBar.generated.h"

class UStatComponent;
class UProgressBar;

UCLASS()
class HYOBINSPROJECT_API UBossStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void BindStatComponent(UStatComponent* statComponent);
	
	void UpdateHPBar(float changeAmount);
	void UpdateHPBarBackground();
	
	void UpdateStaminaBar(float changeAmount);
	void UpdateStaminaBarBackground();

private:
	void updateHPBarBackgroundColor(float uvStart, float uvEnd);
	
private:
	TWeakObjectPtr<UStatComponent> m_StatComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UProgressBar> m_HPProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UProgressBar> m_HPProgressBarBackground;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UProgressBar> m_StaminaProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UProgressBar> m_StaminaProgressBarBackground;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_DecreaseSpeed;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_DecreaseSpeedFirstDelay;
	
	FTimerHandle m_HPBarTimer;
	FTimerHandle m_StaminaBarTimer;
	
	UPROPERTY()
	UMaterialInstanceDynamic* m_DynamicMaterial;
       
};
