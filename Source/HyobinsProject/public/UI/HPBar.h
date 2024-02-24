// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBar.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindStatComponent(class UStatComponent* statComponent);
	void SetExecuteState();

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

protected:
	UPROPERTY(EditDefaultsOnly)
	TWeakObjectPtr<class UStatComponent> m_StatComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TWeakObjectPtr<class UProgressBar> m_HPProgressBar;
};
