// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetPatrolPos_MeleeMinion.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UBTTask_GetPatrolPos_MeleeMinion : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GetPatrolPos_MeleeMinion();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
