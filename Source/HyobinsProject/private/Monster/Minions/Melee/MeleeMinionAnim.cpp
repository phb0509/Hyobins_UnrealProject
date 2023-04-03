// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAnim.h"
#include "Monster/Minions/Melee/MeleeMinion.h"


UMeleeMinionAnim::UMeleeMinionAnim()
{
	loadMontages();
}

void UMeleeMinionAnim::NativeUpdateAnimation(float DeltaSeconds)	
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UMeleeMinionAnim::PlayNormalAttackMontage()
{
	int32 randomIndex = FMath::RandRange(0, 2);

	Montage_Play(m_AttackMontages[randomIndex], 1.0f); // 낮을수록 느리게 재생.
}

void UMeleeMinionAnim::AnimNotify_checkAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnAttackHitCheck.Broadcast();
}

void UMeleeMinionAnim::loadMontages()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage0
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_NormalAttack0.AM_NormalAttack0'"));

	if (normalAttackMontage0.Succeeded())
	{
		m_AttackMontages.Add(normalAttackMontage0.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage1
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_NormalAttack1.AM_NormalAttack1'"));

	if (normalAttackMontage1.Succeeded())
	{
		m_AttackMontages.Add(normalAttackMontage1.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> criticalAttackMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_CriticalAttack.AM_CriticalAttack'"));

	if (criticalAttackMontage.Succeeded())
	{
		m_AttackMontages.Add(criticalAttackMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> onHitMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_HitReact_Front.AM_HitReact_Front'"));

	if (onHitMontage.Succeeded())
	{
		m_OnHitMontages.Add(onHitMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> deathMontage_Front
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AM_Death_Front.AM_Death_Front'"));

	if (deathMontage_Front.Succeeded())
	{
		m_DeathMontages.Add(deathMontage_Front.Object);
	}
}