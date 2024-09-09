// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SuperMinion_SetState.generated.h"

enum class ENormalMinionStates : uint8;

UCLASS()
class HYOBINSPROJECT_API UBTS_SuperMinion_SetState : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_SuperMinion_SetState();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	ENormalMinionStates m_State;
};
