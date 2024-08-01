// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


// NormalAttack
DECLARE_MULTICAST_DELEGATE(FNormalAttack_Start_EachSection_Delegate);

// UpperAttack
DECLARE_MULTICAST_DELEGATE(FUpperAttack_Start_Delegate);

// CombatDodge
DECLARE_MULTICAST_DELEGATE(FCombatDodge_Start_Move_Delegate);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	
	// NormalAttack
	UFUNCTION()
	void AnimNotify_NormalAttack_Start_EachSection() const;
	
	// CombatDodge
	UFUNCTION()
	void AnimNotify_CombatDodge_Start_Move() const;
	

private:
	void initAssets();
	
public:
	// NormalAttack
	FNormalAttack_Start_EachSection_Delegate NormalAttack_Start_EachSection;

	// UpperAttack
	FUpperAttack_Start_Delegate UpperAttack_Start;
	
	// CombatDodge
	FCombatDodge_Start_Move_Delegate CombatDodge_Start_Move;
	
};
