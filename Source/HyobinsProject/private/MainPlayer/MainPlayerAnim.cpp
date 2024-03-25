// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"


UMainPlayerAnim::UMainPlayerAnim() 
{
	initAssets();
	
	UE_LOG(LogTemp, Warning, TEXT("MainPlayerAnim::Constructor"));
}

void UMainPlayerAnim::AnimNotify_CheckNormalAttackHit() const 
{
	OnNormalAttackHitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_CheckNextNormalAttack() const
{
	OnNormalAttackNextCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_EndedNormalAttack() const
{
	OnEndedNormalAttack.Broadcast();
}

void UMainPlayerAnim::AnimNotify_EndedDodgeMove() const
{
	OnEndedDodgeMove.Broadcast();
}

void UMainPlayerAnim::initAssets()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/AM_ComboAttack1.AM_ComboAttack1'"));

	if (normalAttackMontage.Succeeded())
	{
		m_Montages.Add("NormalAttack", normalAttackMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> combatDodgeMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/Dodge_NonCombat.Dodge_NonCombat'"));

	if (combatDodgeMontage.Succeeded())
	{
		m_Montages.Add("Dodge_NonCombat", combatDodgeMontage.Object);
	}
}
