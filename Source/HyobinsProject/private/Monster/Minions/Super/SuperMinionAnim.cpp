// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAnim.h"
#include "Monster/Minions/Super/SuperMinion.h"


USuperMinionAnim::USuperMinionAnim()
{
	initAssets();
}

void USuperMinionAnim::AnimNotify_checkAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnAttackHitCheck.Broadcast();
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


	static ConstructorHelpers::FObjectFinder <UAnimMontage> onHitMontage_Front
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_HitReact_Front.AM_HitReact_Front'"));

	if (onHitMontage_Front.Succeeded())
	{
		m_Montages.Add("OnHit_Front",onHitMontage_Front.Object);
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