// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeMinionAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;


UCLASS()
class HYOBINSPROJECT_API AMeleeMinionAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMeleeMinionAIController();
	virtual void OnPossess(APawn* pawn) override;

protected:
	virtual void BeginPlay() override;


protected:
	int32 m_IEnemyKeyID;

private:
	UPROPERTY(transient) // 해당 속성을 직렬화에서 제외시키는 것이 좋다.
		UBlackboardComponent* m_Blackboard;

	UPROPERTY(transient)
		UBehaviorTreeComponent* m_BehaviorTree;


};
