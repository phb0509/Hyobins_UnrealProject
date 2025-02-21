// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBase.h"
#include "MainPersistantLevel.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AMainPersistantLevel : public ALevelBase
{
	GENERATED_BODY()

public:
	AMainPersistantLevel();

	virtual void BeginPlay() override;
};
