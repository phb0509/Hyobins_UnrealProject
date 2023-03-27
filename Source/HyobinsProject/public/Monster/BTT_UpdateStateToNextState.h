// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UpdateStateToNextState.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UBTT_UpdateStateToNextState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_UpdateStateToNextState();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
