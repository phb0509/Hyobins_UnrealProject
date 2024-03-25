// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAnim.h"


USuperMinionAnim::USuperMinionAnim()
{
	initAssets();
	UE_LOG(LogTemp, Warning, TEXT("SuperMinionAnim::Constructor"));
}


void USuperMinionAnim::AnimNotify_EndedNormalAttack() const
{
	OnEndedNormalAttack.Broadcast();
}

void USuperMinionAnim::initAssets()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage1
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_NormalAttack_A.AM_NormalAttack_A'"));

	if (normalAttackMontage1.Succeeded())
	{
		m_Montages.Add("NormalAttack1", normalAttackMontage1.Object);
	}

	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage2
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_NormalAttack_B.AM_NormalAttack_B'"));

	if (normalAttackMontage2.Succeeded())
	{
		m_Montages.Add("NormalAttack2",normalAttackMontage2.Object);
	}


	static ConstructorHelpers::FObjectFinder <UAnimMontage> criticalAttackMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_CriticalAttack.AM_CriticalAttack'"));

	if (criticalAttackMontage.Succeeded())
	{
		m_Montages.Add("CriticalAttack",criticalAttackMontage.Object);
	}

	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> onDeath_Front
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_Death_Front.AM_Death_Front'"));

	if (onDeath_Front.Succeeded())
	{
		m_Montages.Add("OnDeath_Front",onDeath_Front.Object);
	}


	static ConstructorHelpers::FObjectFinder <UAnimMontage> onDeath_Back
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_Death_Back.AM_Death_Back'"));

	if (onDeath_Back.Succeeded())
	{
		m_Montages.Add("OnDeath_Back",onDeath_Back.Object);
	}
}
