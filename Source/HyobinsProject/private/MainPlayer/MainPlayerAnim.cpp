// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim() 
{
	initAssets();
	
}

void UMainPlayerAnim::AnimNotify_checkNormalAttackHit() //  
{
	OnNormalAttackHitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_checkNextNormalAttack()
{
	OnNormalAttackNextCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_EndedNormalAttack()
{
	OnEndedNormalAttack.Broadcast();
}

void UMainPlayerAnim::AnimNotify_EndedDodgeMove()
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
