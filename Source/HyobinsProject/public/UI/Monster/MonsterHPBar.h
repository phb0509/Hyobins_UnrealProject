// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBar.generated.h"

class UStatComponent;
//class UOverlay;
class UProgressBar;
class UImage;


UCLASS()
class HYOBINSPROJECT_API UMonsterHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void BindStatComponent(UStatComponent* ownerStatComponent);
	void InitHPBar();

	void OnDamagedHPBar();
	void OnRecoveredHPBar();
	
	void OnStaminaIsZero();

private:
	void updateGuardRegainProgressBar();
	void startWhiteBlink();
	
protected:
	TWeakObjectPtr<UStatComponent> m_OwnerStatComponent;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	// TObjectPtr<UOverlay> m_HPBarOverlay;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> m_HPProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> m_GuardRegainProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UImage> m_HPBarBorder;
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = true))
	float m_DecreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = true))
	float m_DecreaseSpeedFirstDelay;
	
	FTimerHandle m_HPBarTimer;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
    TObjectPtr<UWidgetAnimation> m_GroggyAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInstance> m_HPBarBorderMaterialInstance;

	TWeakObjectPtr<UTexture2D> m_HPBarBorderTexture2D;

	TWeakObjectPtr<UMaterialInstanceDynamic> m_HPProgressBarMaterialInstanceDynamic;
	FTimerHandle m_WhiteBlinkTimer;
	float m_WhiteBlinkLerpAlpha;
	float m_WhiteBlnkStopAccumulatedTime;
	int8 m_WhiteBlinkDirection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float m_WhiteBlinkSpeed;
};
