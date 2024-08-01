// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "SuperMinionAnim.generated.h"

UCLASS()
class HYOBINSPROJECT_API USuperMinionAnim : public UAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	USuperMinionAnim();

private:
	void initAssets();
	
	UFUNCTION()
	void AnimNotify_Test() const;

private:
	static const FName HitMontages[4]; 
	static const FName GroggyMontage; 
	static const FName DeathMontages[4];

};
