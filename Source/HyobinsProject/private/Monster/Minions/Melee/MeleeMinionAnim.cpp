// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Melee/MeleeMinionAnim.h"


UMeleeMinionAnim::UMeleeMinionAnim()
{
	initAssets();
}

//void UMeleeMinionAnim::PlayNormalAttackMontage(float InPlayRate)
//{
//	int32 randomIndex = FMath::RandRange(0, 2);
//
//	Montage_Play(m_AttackMontages[randomIndex], InPlayRate); // 낮을수록 느리게 재생.
//}

void UMeleeMinionAnim::AnimNotify_checkAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnAttackHitCheck.Broadcast();
}

void UMeleeMinionAnim::initAssets()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage1
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_NormalAttack0.AM_NormalAttack0'"));

	if (normalAttackMontage1.Succeeded())
	{
		m_Montages.Add("NormalAttack1",normalAttackMontage1.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage2
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_NormalAttack1.AM_NormalAttack1'"));

	if (normalAttackMontage2.Succeeded())
	{
		m_Montages.Add("NormalAttack2",normalAttackMontage2.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> criticalAttackMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_CriticalAttack.AM_CriticalAttack'"));

	if (criticalAttackMontage.Succeeded())
	{
		m_Montages.Add("CriticalAttack",criticalAttackMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> onHitMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_HitReact_Front.AM_HitReact_Front'"));

	if (onHitMontage.Succeeded())
	{
		m_Montages.Add("OnHit_Front",onHitMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> deathMontage_Front
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_Death_Front.AM_Death_Front'"));

	if (deathMontage_Front.Succeeded())
	{
		m_Montages.Add("OnDeath_Front",deathMontage_Front.Object);
	}

}