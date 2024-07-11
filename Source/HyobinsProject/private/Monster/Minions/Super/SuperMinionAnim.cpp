// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAnim.h"


USuperMinionAnim::USuperMinionAnim()
{
	initAssets();
}

void USuperMinionAnim::AnimNotify_Test() const
{
	UE_LOG(LogTemp, Warning, TEXT("OnCalled Test Notify"));
}

void USuperMinionAnim::initAssets()
{
	// NormalAttack
	static ConstructorHelpers::FObjectFinder <UAnimMontage> NormalAttack0
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/NormalAttack0.NormalAttack0'"));
	if (NormalAttack0.Succeeded())
	{
		m_Montages.Add("NormalAttack0",NormalAttack0.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> NormalAttack1
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/NormalAttack1.NormalAttack1'"));
	if (NormalAttack1.Succeeded())
	{
		m_Montages.Add("NormalAttack1",NormalAttack1.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> CriticalAttack
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/CriticalAttack.CriticalAttack'"));
	if (CriticalAttack.Succeeded())
	{
		m_Montages.Add("CriticalAttack",CriticalAttack.Object);
	}

	
	// OnHit
	static ConstructorHelpers::FObjectFinder <UAnimMontage> OnHit0
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/OnHit0.OnHit0'"));
	if (OnHit0.Succeeded())
	{
		m_Montages.Add("OnHit0",OnHit0.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> OnHit1
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/OnHit1.OnHit1'"));
	if (OnHit1.Succeeded())
	{
		m_Montages.Add("OnHit1",OnHit1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> OnHit2
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/OnHit2.OnHit2'"));
	if (OnHit2.Succeeded())
	{
		m_Montages.Add("OnHit2",OnHit2.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> OnHit3
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/OnHit3.OnHit3'"));
	if (OnHit3.Succeeded())
	{
		m_Montages.Add("OnHit3",OnHit3.Object);
	}

	
	// Death
	static ConstructorHelpers::FObjectFinder <UAnimMontage> Death0 
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/Death0.Death0'"));
	if (Death0.Succeeded())
	{
		m_Montages.Add("Death0",Death0.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> Death1
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/Death1.Death1'"));
	if (Death1.Succeeded())
	{
		m_Montages.Add("Death1",Death1.Object);
	}
	
}
