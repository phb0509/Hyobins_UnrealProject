// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MontageTaskNodeBase.h"
#include "BTT_LichKing_SoulSiphon.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UBTT_LichKing_SoulSiphon : public UMontageTaskNodeBase
{
	GENERATED_BODY()
	
public:
	UBTT_LichKing_SoulSiphon();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const;
};
