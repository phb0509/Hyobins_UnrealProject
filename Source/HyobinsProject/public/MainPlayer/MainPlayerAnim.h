// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;

DECLARE_MULTICAST_DELEGATE(FSwordCollider_Start_HitCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FSwordCollider_End_HitCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FShieldForAttackCollider_Start_HitCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FShieldForAttackCollider_End_HitCheck_Delegate);

// NormalAttack
DECLARE_MULTICAST_DELEGATE(FNormalAttack_Start_EachSection_Delegate);
DECLARE_MULTICAST_DELEGATE(FNormalAttack_KeyInputCheck_Delegate);
DECLARE_MULTICAST_DELEGATE(FNormalAttack_End_EachSection_Delegate);

// UpperAttack
DECLARE_MULTICAST_DELEGATE(FUpperAttack_Start_Delegate);
DECLARE_MULTICAST_DELEGATE(FUpperAttack_End_Delegate);

// CombatDodge
DECLARE_MULTICAST_DELEGATE(FCombatDodge_Start_Move_Delegate);
DECLARE_MULTICAST_DELEGATE(FCombatDodge_End_Move_Delegate);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	
	// NormalAttack
	UFUNCTION()
	void AnimNotify_NormalAttack_Start_EachSection() const;
	
	UFUNCTION()
	void AnimNotify_NormalAttack_KeyInputCheck() const;

	UFUNCTION()
	void AnimNotify_NormalAttack_End_EachSection() const; 


	// UpperAttack
	UFUNCTION()
	void AnimNotify_UpperAttack_Start() const;
	
	UFUNCTION()
	void AnimNotify_UpperAttack_End() const; 


	// CombatDodge
	UFUNCTION()
	void AnimNotify_CombatDodge_Start_Move() const;
	
	UFUNCTION()
	void AnimNotify_CombatDodge_End_Move() const; 

	// 각 충돌체들
	UFUNCTION()
	void AnimNotify_SwordCollider_Start_HitCheck() const; 

	UFUNCTION()
	void AnimNotify_SwordCollider_End_HitCheck() const;
	
	UFUNCTION()
	void AnimNotify_ShieldForAttackCollider_Start_HitCheck() const; 

	UFUNCTION()
	void AnimNotify_ShieldForAttackCollider_End_HitCheck() const;
	
private:
	void initAssets();

	
public:
	// NormalAttack
	FNormalAttack_Start_EachSection_Delegate NormalAttack_Start_EachSection;
	FNormalAttack_KeyInputCheck_Delegate NormalAttack_KeyInputCheck;
	FNormalAttack_End_EachSection_Delegate NormalAttack_End_EachSection;

	// UpperAttack
	FUpperAttack_Start_Delegate UpperAttack_Start;
	FUpperAttack_End_Delegate UpperAttack_End;
	
	// CombatDodge
	FCombatDodge_Start_Move_Delegate CombatDodge_Start_Move;
	FCombatDodge_End_Move_Delegate CombatDodge_End_Move;

	// 각 충돌체들
	FSwordCollider_Start_HitCheck_Delegate SwordCollider_Start_HitCheck;
	FSwordCollider_End_HitCheck_Delegate SwordCollider_End_HitCheck;
	FShieldForAttackCollider_Start_HitCheck_Delegate ShieldForAttackCollider_Start_HitCheck;
	FShieldForAttackCollider_End_HitCheck_Delegate ShieldForAttackCollider_End_HitCheck;
};
