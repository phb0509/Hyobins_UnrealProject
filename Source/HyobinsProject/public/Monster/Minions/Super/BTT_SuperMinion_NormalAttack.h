// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SuperMinion_NormalAttack.generated.h"


UCLASS()
class HYOBINSPROJECT_API UBTT_SuperMinion_NormalAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SuperMinion_NormalAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	static FName m_NormalAttackNames[2];
	
};
