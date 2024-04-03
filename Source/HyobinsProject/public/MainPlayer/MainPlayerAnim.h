// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;

DECLARE_MULTICAST_DELEGATE(FNormalAttack_Start_EachSection_Delegate);
DECLARE_MULTICAST_DELEGATE(FNormalAttack_Start_HitCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FNormalAttack_End_HitCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FNormalAttack_KeyInputCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FNormalAttack_End_EachSection_Delegate);

DECLARE_MULTICAST_DELEGATE(FCombatDodge_End_Move_Delegate);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	
	UFUNCTION()
	void AnimNotify_NormalAttack_Start_EachSection() const; 
	
	UFUNCTION()
	void AnimNotify_NormalAttack_Start_HitCheck() const; 

	UFUNCTION()
	void AnimNotify_NormalAttack_End_HitCheck() const; 

	UFUNCTION()
	void AnimNotify_NormalAttack_KeyInputCheck() const;

	UFUNCTION()
	void AnimNotify_NormalAttack_End_EachSection() const; 

	
	UFUNCTION()
	void AnimNotify_CombatDodge_End_Move() const; 

	
private:
	void initAssets();

	
public:
	FNormalAttack_Start_EachSection_Delegate NormalAttack_Start_EachSection;
	FNormalAttack_Start_HitCheck_Delegate NormalAttack_Start_HitCheck;
	FNormalAttack_End_HitCheck_Delegate NormalAttack_End_HitCheck;
	FNormalAttack_KeyInputCheck_Delegate NormalAttack_KeyInputCheck;
	FNormalAttack_End_EachSection_Delegate NormalAttack_End_EachSection;
	
	FCombatDodge_End_Move_Delegate CombatDodge_End_Move;
};
