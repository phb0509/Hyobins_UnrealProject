// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"

UMainPlayerAnim::UMainPlayerAnim() 
{
	initAssets();
}

void UMainPlayerAnim::AnimNotify_NormalAttack_Start_EachSection() const
{
	NormalAttack_Start_EachSection.Broadcast();
}

void UMainPlayerAnim::AnimNotify_UpperAttack_Start() const
{
	UpperAttack_Start.Broadcast();
}

void UMainPlayerAnim::AnimNotify_CombatDodge_Start_Move() const
{
	CombatDodge_Start_Move.Broadcast();
}

void UMainPlayerAnim::AnimNotify_SwordCollider_Start_HitCheck() const 
{
	SwordCollider_Start_HitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_SwordCollider_End_HitCheck() const
{
	SwordCollider_End_HitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_ShieldForAttackCollider_Start_HitCheck() const
{
	ShieldForAttackCollider_Start_HitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_ShieldForAttackCollider_End_HitCheck() const
{
	ShieldForAttackCollider_End_HitCheck.Broadcast();
}

void UMainPlayerAnim::initAssets()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> NormalAttack_OnGround
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/NormalAttack_OnGround.NormalAttack_OnGround'"));
	if (NormalAttack_OnGround.Succeeded())
	{
		m_Montages.Add("NormalAttack_OnGround",NormalAttack_OnGround.Object);
	}

	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> CombatDodge_OnGround
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/CombatDodge_OnGround.CombatDodge_OnGround'"));
	if (CombatDodge_OnGround.Succeeded())
	{
		m_Montages.Add("CombatDodge_OnGround",CombatDodge_OnGround.Object);
	}
}


