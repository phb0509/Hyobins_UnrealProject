// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterUpperHPBar.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UCharacterUpperHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindStatComponent(class UStatComponent* statComponent);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UStatComponent> m_StatComponent;

	UPROPERTY()
		class UProgressBar* m_HPProgressBar;
};
