// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	
	UFUNCTION(BlueprintCallable)
	void ExecEvent_OnEnteredState_Falling() const;

	UFUNCTION(BlueprintCallable)
	void ExecEvent_OnEnteredState_MoveOnGround() const;
	
};
