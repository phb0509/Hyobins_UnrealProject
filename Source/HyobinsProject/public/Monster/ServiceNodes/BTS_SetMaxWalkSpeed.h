// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetMaxWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UBTS_SetMaxWalkSpeed : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_SetMaxWalkSpeed();
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	//virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	
private:
	UPROPERTY(EditAnywhere)
	float m_MaxWalkSpeed;
};
