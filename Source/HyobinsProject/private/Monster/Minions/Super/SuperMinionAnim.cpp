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

void USuperMinionAnim::AnimNotify_Test() const
{
	UE_LOG(LogTemp, Warning, TEXT("OnCalled Test Notify"));
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

	static ConstructorHelpers::FObjectFinder <UAnimMontage> onHit_OnGround
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/OnHit_OnGround.OnHit_OnGround'"));
	
	if (onHit_OnGround.Succeeded())
	{
		m_Montages.Add("OnHit_OnGround",onHit_OnGround.Object);
	}

	
	static ConstructorHelpers::FObjectFinder <UAnimMontage> death_OnGround
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/Death_OnGround.Death_OnGround'"));

	if (death_OnGround.Succeeded())
	{
		m_Montages.Add("Death_OnGround",death_OnGround.Object);
	}
}
