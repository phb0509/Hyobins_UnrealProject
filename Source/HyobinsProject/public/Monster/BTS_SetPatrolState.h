// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetPatrolState.generated.h"

enum class EMonsterCommonStates : uint8;

UCLASS()
class HYOBINSPROJECT_API UBTS_SetPatrolState : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_SetPatrolState();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
