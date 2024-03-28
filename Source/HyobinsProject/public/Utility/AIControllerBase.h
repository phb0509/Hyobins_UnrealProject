// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
//#include "DetourCrowdAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIControllerBase.generated.h"

/**
 *
 */
UCLASS()
class HYOBINSPROJECT_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	void StopBehaviorTree() const;
	void StartBehaviorTree();

	UFUNCTION()
	virtual void UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus) {};

    // Get
	FORCEINLINE UBlackboardComponent* GetBlackBoard() const { return Blackboard; }


protected:
	TWeakObjectPtr<UBehaviorTree> m_BehaviorTree;
	TWeakObjectPtr<UBlackboardData> m_BlackboardData;
	TWeakObjectPtr<UBehaviorTreeComponent> m_BehaviorTreeComponent;
	TWeakObjectPtr<UAIPerceptionComponent> m_AIPerceptionComponent;
	TWeakObjectPtr<UAISenseConfig_Sight> m_SightConfig;

	FGenericTeamId m_TeamID;
	
	float m_SightRadius; // 인지최대거리
	float m_LoseSightRadius; // 이미 본 타깃을 시야에서 놓치게 되는 시야최대거리. (시야상실반경)
	float m_PeripheralVisionHalfAngle; // FieldOfView
	float m_AISightAge;
	float m_AILastSeenLocation;

};
