// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"


UMainPlayerAnim::UMainPlayerAnim() 
{
	initAssets();
	
	UE_LOG(LogTemp, Warning, TEXT("MainPlayerAnim::Constructor"));
}

void UMainPlayerAnim::AnimNotify_NormalAttack_Start_EachSection() const
{
	NormalAttack_Start_EachSection.Broadcast();
}

void UMainPlayerAnim::AnimNotify_NormalAttack_KeyInputCheck() const
{
	NormalAttack_KeyInputCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_NormalAttack_End_EachSection() const
{
	NormalAttack_End_EachSection.Broadcast();
}

void UMainPlayerAnim::AnimNotify_UpperAttack_Start() const
{
	UpperAttack_Start.Broadcast();
}

void UMainPlayerAnim::AnimNotify_UpperAttack_End() const
{
	UpperAttack_End.Broadcast();
}

void UMainPlayerAnim::AnimNotify_CombatDodge_End_Move() const
{
	CombatDodge_End_Move.Broadcast();
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
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/AM_ComboAttack1.AM_ComboAttack1'"));
	
	if (normalAttackMontage.Succeeded())
	{
		m_Montages.Add("NormalAttack", normalAttackMontage.Object);
	}

	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> upperAttackMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/AM_Attack_Up_OnGround.AM_Attack_Up_OnGround'"));
	
	if (upperAttackMontage.Succeeded())
	{
		m_Montages.Add("UpperAttack", upperAttackMontage.Object);
	}
}
