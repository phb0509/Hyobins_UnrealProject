// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableActor : public UInterface
{
	GENERATED_BODY()
};

class HYOBINSPROJECT_API IPoolableActor 
{
	GENERATED_BODY()

public:
	virtual void Initialize() abstract;
	virtual void Activate() abstract;
	virtual void Deactivate() abstract;
	virtual bool IsActive() abstract;

	
};