// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerBase();

protected:
	void LoadBehaviorTree(FString assetPath);
	void LoadBlackBoard(FString assetPath);

protected:
	UPROPERTY()
		class UBehaviorTree* m_BehaviorTree;
	UPROPERTY()
		class UBlackboardData* m_BlackboardData;
	UPROPERTY()
		class UBehaviorTreeComponent* m_BehaviorTreeComponent;
};
