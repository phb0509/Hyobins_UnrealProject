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

void UMainPlayerAnim::AnimNotify_CombatDodge_Start_Move() const
{
	CombatDodge_Start_Move.Broadcast();
}

void UMainPlayerAnim::initAssets()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> NormalAttack
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/NormalAttack.NormalAttack'"));
	if (NormalAttack.Succeeded())
	{
		m_Montages.Add("NormalAttack",NormalAttack.Object);
	}
	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> Dodge_NonTargeting
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/Dodge_NonTargeting.Dodge_NonTargeting'"));
	if (Dodge_NonTargeting.Succeeded())
	{
		m_Montages.Add("Dodge_NonTargeting",Dodge_NonTargeting.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> Dodge_Targeting
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/Dodge_Targeting.Dodge_Targeting'"));
	if (Dodge_Targeting.Succeeded())
	{
		m_Montages.Add("Dodge_Targeting",Dodge_Targeting.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> Parry
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/Parry.Parry'"));
	if (Parry.Succeeded())
	{
		m_Montages.Add("Parry",Parry.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> ParryingAttack
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/ParryingAttack.ParryingAttack'"));
	if (ParryingAttack.Succeeded())
	{
		m_Montages.Add("ParryingAttack",ParryingAttack.Object);
	}
}


