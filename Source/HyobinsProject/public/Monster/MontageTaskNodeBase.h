// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MontageTaskNodeBase.generated.h"


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
	
	virtual uint16 GetInstanceMemorySize() const override;
	
};
