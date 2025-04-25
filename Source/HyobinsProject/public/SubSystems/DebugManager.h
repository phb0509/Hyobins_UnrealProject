// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDebugModeDelegate);

UCLASS()
class HYOBINSPROJECT_API UDebugManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDebugManager();
	
	UFUNCTION(BlueprintCallable, category = "Debugging")
	void ToggleDebugMode();


public:
	FOnDebugModeDelegate OnDebugMode;


private:
	bool m_bIsDebugMode;
	FTimerHandle m_LogTimer;

};


