// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;

private:
	UPROPERTY()
		class UBehaviorTree* m_BehaviorTree;
	UPROPERTY()
		class UBlackboardData* m_BlackboardData;
	UPROPERTY()
		class UBehaviorTreeComponent* m_BehaviorTreeComponent;
};
