// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AIControllerBase.h"
#include "MeleeMinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AMeleeMinionAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AMeleeMinionAIController();
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;

	void initPerceptionSystem();


	static const FName HomePosKey;
	static const FName PatrolPosKey;

};
