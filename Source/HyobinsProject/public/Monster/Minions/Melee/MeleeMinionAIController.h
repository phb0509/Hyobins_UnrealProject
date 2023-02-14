// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeMinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AMeleeMinionAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMeleeMinionAIController();

protected:
	virtual void BeginPlay() override;
	
};
