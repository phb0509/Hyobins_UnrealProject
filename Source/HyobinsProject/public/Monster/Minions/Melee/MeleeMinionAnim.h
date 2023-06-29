// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MeleeMinionAnim.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class HYOBINSPROJECT_API UMeleeMinionAnim : public UAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	UMeleeMinionAnim();


private:
	void initAssets();

	UFUNCTION()
		void AnimNotify_checkAttackHit(); // Notify를 실행시키는 함수.

public:
	FOnAttackHitCheckDelegate  OnAttackHitCheck;

};
