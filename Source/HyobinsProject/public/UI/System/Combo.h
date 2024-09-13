// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Combo.generated.h"

class UTextBlock;
class UStatComponent;


UCLASS()
class HYOBINSPROJECT_API UCombo : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void BindStatComponent(UStatComponent* const statComponent);

private:
	void updateComboCount();
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> m_ComboCountText;
	FTimerHandle m_ComboCountTimer;
	int32 m_ComboCount;

	UPROPERTY(EditAnywhere)
	float m_ComboCountDecisionTime;

	UPROPERTY(Transient, meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> m_ComboAnimation;
};
