// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


// NormalAttack
DECLARE_MULTICAST_DELEGATE(FNormalAttack_Start_EachSection);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnteredState_Falling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnteredState_MoveOnGround);

UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	
	// NormalAttack
	UFUNCTION()
	void AnimNotify_NormalAttack_Start_EachSection() const;
	
	UFUNCTION(BlueprintCallable)
	void ExecEvent_OnEnteredState_Falling() const;

	UFUNCTION(BlueprintCallable)
	void ExecEvent_OnEnteredState_MoveOnGround() const;

public:
	FNormalAttack_Start_EachSection NormalAttack_Start_EachSection;
	
	FOnEnteredState_Falling OnEnteredState_Falling;
	FOnEnteredState_MoveOnGround OnEnteredState_MoveOnGround;
};
