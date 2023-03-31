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
	//AAIControllerBase();
	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	void StopBehaviorTree();
	void PlayBehaviorTree();

protected:
	void LoadBehaviorTree(FString assetPath);
	void LoadBlackBoard(FString assetPath);

	UBlackboardComponent* GetBlackBoard() { return Blackboard; }
	

protected:
	UPROPERTY()
		UBehaviorTree* m_BehaviorTree;
	UPROPERTY()
		UBlackboardData* m_BlackboardData;
	UPROPERTY()
		UBehaviorTreeComponent* m_BehaviorTreeComponent;
	UPROPERTY()
		UAIPerceptionComponent* m_AIPerceptionComponent;
	UPROPERTY()
		UAISenseConfig_Sight* m_SightConfig;

	FGenericTeamId m_TeamID;
	float m_SightRadius; // �����ִ�Ÿ�
	float m_LoseSightRadius; // �̹� �� Ÿ���� �þ߿��� ��ġ�� �Ǵ� �þ��ִ�Ÿ�. (�þ߻�ǹݰ�)
	float m_PeripheralVisionHalfAngle; // FieldOfView
	float m_AISightAge;
	float m_AILastSeenLocation;

};
