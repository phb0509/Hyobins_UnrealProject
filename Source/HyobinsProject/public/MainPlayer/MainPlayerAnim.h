// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


// NormalAttack
DECLARE_MULTICAST_DELEGATE(FNormalAttack_Start_EachSection_Delegate);

// UpperAttack
DECLARE_MULTICAST_DELEGATE(FAirToGroundAttack_CollisionCheck);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	
	// NormalAttack
	UFUNCTION()
	void AnimNotify_NormalAttack_Start_EachSection() const;

    // AirToGroundAttack
	UFUNCTION()
	void AnimNotify_AirToGroundAttack_CollisionCheck() const;
	



public:
	FNormalAttack_Start_EachSection_Delegate NormalAttack_Start_EachSection;
	FAirToGroundAttack_CollisionCheck AirToGroundAttack_CollisionCheck;
	
	
};
