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
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/NormalAttack_OnGround.NormalAttack_OnGround'"));
	
	if (normalAttackMontage.Succeeded())
	{
		m_Montages.Add("NormalAttack_OnGround", normalAttackMontage.Object);
	}
	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> upperAttackMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/UpperAttack_OnGround.UpperAttack_OnGround'"));
	
	if (upperAttackMontage.Succeeded())
	{
		m_Montages.Add("UpperAttack_OnGround", upperAttackMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> combatDodgeMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/CombatDodge_OnGround.CombatDodge_OnGround'"));
	
	if (combatDodgeMontage.Succeeded())
	{
		m_Montages.Add("CombatDodge_OnGround", combatDodgeMontage.Object);
	}
}
