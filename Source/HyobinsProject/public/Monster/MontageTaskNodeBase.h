// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MontageTaskNodeBase.generated.h"

class ACharacterBase;
class UAnimInstanceBase;

struct FInstanceNode
{
	bool bHasInit = false;
};

UCLASS()
class HYOBINSPROJECT_API UMontageTaskNodeBase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMontageTaskNodeBase();
	
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual FORCEINLINE uint16 GetInstanceMemorySize()  const override
	{
		return sizeof(FInstanceNode);
	}

};
