// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

struct FAttackInformation;
class IAttacker;

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class HYOBINSPROJECT_API IDamageable
{
	GENERATED_BODY()

public:
	virtual void OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation*, AActor* instigator, const FVector& causerLocation) abstract;

};
