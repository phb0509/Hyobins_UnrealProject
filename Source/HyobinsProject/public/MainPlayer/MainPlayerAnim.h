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
	
	// UpperAttack
	UFUNCTION()
	void AnimNotify_UpperAttack_Start() const;

	
	// CombatDodge
	UFUNCTION()
	void AnimNotify_CombatDodge_Start_Move() const;
	

	// �� �浹ü��
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

	// UpperAttack
	FUpperAttack_Start_Delegate UpperAttack_Start;
	
	// CombatDodge
	FCombatDodge_Start_Move_Delegate CombatDodge_Start_Move;

	// �� �浹ü��
	FSwordCollider_Start_HitCheck_Delegate SwordCollider_Start_HitCheck;
	FSwordCollider_End_HitCheck_Delegate SwordCollider_End_HitCheck;
	FShieldForAttackCollider_Start_HitCheck_Delegate ShieldForAttackCollider_Start_HitCheck;
	FShieldForAttackCollider_End_HitCheck_Delegate ShieldForAttackCollider_End_HitCheck;
};
