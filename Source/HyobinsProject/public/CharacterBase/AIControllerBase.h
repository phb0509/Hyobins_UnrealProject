// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIControllerBase.generated.h"

class ACharacterBase;
class UAIPerceptionComponent;


UCLASS()
class HYOBINSPROJECT_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;
	
	void StopBehaviorTree();
	void StartBehaviorTree();
	void RestartBehaviorTree();
	
    // Get
	FORCEINLINE UBlackboardComponent* GetBlackBoard() const { return Blackboard; }


protected:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	TWeakObjectPtr<UBehaviorTree> m_BehaviorTree;
	TWeakObjectPtr<UBlackboardData> m_BlackboardData;
	TWeakObjectPtr<UBehaviorTreeComponent> m_BehaviorTreeComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionComponent> m_AIPerceptionComponent;
	
	FGenericTeamId m_TeamID;
};
