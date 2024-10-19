// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindPatrolPos.generated.h"


UCLASS()
class HYOBINSPROJECT_API UBTS_FindPatrolPos : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_FindPatrolPos();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float m_PatrolRange;
};
