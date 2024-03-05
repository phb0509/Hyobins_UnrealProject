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
	void SetExecuteState(); // 처형표시 (hp일정이하 내려갔을 시,)

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

protected:
	UPROPERTY(EditDefaultsOnly)
	TWeakObjectPtr<UStatComponent> m_StatComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TWeakObjectPtr<class UProgressBar> m_HPProgressBar;
};
