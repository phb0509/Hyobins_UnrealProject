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
	virtual void Activate() = 0;
	virtual void DeActivate() = 0;
	bool GetIsActivated() { return m_bIsActivated; }

protected:
	bool m_bIsActivated = false;
};