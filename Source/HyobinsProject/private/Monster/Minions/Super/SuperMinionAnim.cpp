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

void USuperMinionAnim::PlayNormalAttackMontage(float InPlayRate)
{
	int32 randomIndex = FMath::RandRange(0, 2);

	Montage_Play(m_AttackMontages[randomIndex], InPlayRate); // �������� ������ ���.
}

void USuperMinionAnim::AnimNotify_checkAttackHit() // ��Ƽ���� ���� �Լ�. ��Ÿ�������� ��Ƽ�����̸��� �����ϰ� �����ؾ��Ѵ�.
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

	static ConstructorHelpers::FObjectFinder <UAnimMontage> death_Front
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_Death_Front.AM_Death_Front'"));

	if (death_Front.Succeeded())
	{
		m_DeathMontages.Add(death_Front.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> death_Back
	(TEXT("AnimMontage'/Game/MonsterAsset/SuperMinion/AM_Death_Back.AM_Death_Back'"));

	if (death_Back.Succeeded())
	{
		m_DeathMontages.Add(death_Back.Object);
	}
}