// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerStatusBar.generated.h"

class UProgressBar;
class UStatComponent;

UCLASS()
class HYOBINSPROJECT_API UMainPlayerStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void BindStatComponent(UStatComponent* statComponent);
	void UpdateHPBar();
	void UpdateStaminaBar();

	
private:
	TWeakObjectPtr<UStatComponent> m_OwnerStatComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> m_HPProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> m_StaminaProgressBar;
	
};
