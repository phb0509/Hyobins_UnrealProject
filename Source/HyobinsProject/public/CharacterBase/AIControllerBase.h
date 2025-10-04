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
	
	void StartBehaviorTree();
	void StopBehaviorTree();
	void RestartBehaviorTree() const;
	
    // Get
	FORCEINLINE UBlackboardComponent* GetBlackBoard() const { return Blackboard; }


protected:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> m_BehaviorTree;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBlackboardData> m_BlackboardData;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTreeComponent> m_BehaviorTreeComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionComponent> m_AIPerceptionComponent;
	
	FGenericTeamId m_TeamID;
};
