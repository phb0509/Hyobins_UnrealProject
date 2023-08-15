// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Component/StatComponent.h"
#include "StatusActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatusActor : public UInterface
{
	GENERATED_BODY()
};


class HYOBINSPROJECT_API IStatusActor
{
	GENERATED_BODY()

public:
	UStatComponent* GetStatComponent() { return m_StatComponent; }

protected:
	UStatComponent* m_StatComponent;
};
