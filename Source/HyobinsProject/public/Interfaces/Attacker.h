// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Attacker.generated.h"

// This class does not need to be modified.


UINTERFACE(MinimalAPI)
class UAttacker : public UInterface
{
	GENERATED_BODY()
};


class HYOBINSPROJECT_API IAttacker
{
	GENERATED_BODY()

public:
	virtual void Attack(const FName& attackName, AActor* target, const FVector& causerLocation) abstract;
};
