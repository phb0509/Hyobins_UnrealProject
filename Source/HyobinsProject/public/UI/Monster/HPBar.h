// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBar.generated.h"

class UStatComponent;
class UProgressBar;


UCLASS()
class HYOBINSPROJECT_API UHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void BindStatComponent(UStatComponent* statComponent);
	void UpdateHPBar(float changeAmount);
	
private:
	TWeakObjectPtr<UStatComponent> m_StatComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> m_HPProgressBar;
	
};
