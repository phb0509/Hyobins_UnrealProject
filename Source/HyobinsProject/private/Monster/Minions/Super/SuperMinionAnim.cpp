// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAnim.h"
#include "Monster/Minions/Super/SuperMinion.h"


USuperMinionAnim::USuperMinionAnim()
{
	loadMontages();
}

void USuperMinionAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void USuperMinionAnim::PlayNormalAttackMontage()
{
	int32 randomIndex = FMath::RandRange(0, 2);

	Montage_Play(m_AttackMontages[randomIndex], 1.0f); // 낮을수록 느리게 재생.
}

void USuperMinionAnim::AnimNotify_checkAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnAttackHitCheck.Broadcast();
}

void USuperMinionAnim::loadMontages()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage0
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_NormalAttack_A.AM_NormalAttack_A'"));

	if (normalAttackMontage0.Succeeded())
	{
		m_AttackMontages.Add(normalAttackMontage0.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage1
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_NormalAttack_B.AM_NormalAttack_B'"));

	if (normalAttackMontage1.Succeeded())
	{
		m_AttackMontages.Add(normalAttackMontage1.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> criticalAttackMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_CriticalAttack.AM_CriticalAttack'"));

	if (criticalAttackMontage.Succeeded())
	{
		m_AttackMontages.Add(criticalAttackMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> onHitMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_HitReact_Front.AM_HitReact_Front'"));

	if (onHitMontage.Succeeded())
	{
		m_OnHitMontages.Add(onHitMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimSequence> death_Front
	(TEXT("AnimSequence'/Game/MonsterAsset/SuperMinion/Death_Front.Death_Front'"));

	if (death_Front.Succeeded())
	{
		m_DeathSequences.Add(death_Front.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimSequence> death_Back
	(TEXT("AnimSequence'/Game/MonsterAsset/SuperMinion/Death_Back.Death_Back'"));

	if (death_Back.Succeeded())
	{
		m_DeathSequences.Add(death_Back.Object);
	}
}