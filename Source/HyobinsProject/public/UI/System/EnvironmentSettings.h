// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnvironmentSettings.generated.h"

class UCheckBox;


UCLASS()
class HYOBINSPROJECT_API UEnvironmentSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ChangeMonsterHPBarState(bool bIsChecked);

private:
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCheckBox> m_bHideCheckBox;

	
};
