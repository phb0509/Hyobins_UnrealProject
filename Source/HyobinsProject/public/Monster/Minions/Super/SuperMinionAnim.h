// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "SuperMinionAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEndedNormalAttackDelegate);

UCLASS()
class HYOBINSPROJECT_API USuperMinionAnim : public UAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	USuperMinionAnim();


private:
	void initAssets();
	
	UFUNCTION()
	void AnimNotify_EndedNormalAttack() const; 

public:
	FOnEndedNormalAttackDelegate OnEndedNormalAttack;

};
