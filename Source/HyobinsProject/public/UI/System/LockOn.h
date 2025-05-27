// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOn.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API ULockOn : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


};
