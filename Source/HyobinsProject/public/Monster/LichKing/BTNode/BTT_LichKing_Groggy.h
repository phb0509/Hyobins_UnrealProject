// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MontageTaskNodeBase.h"
#include "BTT_LichKing_Groggy.generated.h"


UCLASS()
class HYOBINSPROJECT_API UBTT_LichKing_Groggy : public UMontageTaskNodeBase
{
	GENERATED_BODY()

public:
	UBTT_LichKing_Groggy();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const;

private:
	UPROPERTY(EditAnywhere)
	float m_GroggyTime;
};
