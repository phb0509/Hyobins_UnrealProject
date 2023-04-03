// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "SuperMinionAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class HYOBINSPROJECT_API USuperMinionAnim : public UAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	USuperMinionAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayNormalAttackMontage();

private:
	void loadMontages();

	UFUNCTION()
		void AnimNotify_checkAttackHit(); // Notify를 실행시키는 함수.

public:
	FOnAttackHitCheckDelegate  OnAttackHitCheck;

};
