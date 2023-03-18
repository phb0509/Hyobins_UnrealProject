// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAnim.h"
#include "Monster/Minions/Melee/MeleeMinion.h"


UMeleeMinionAnim::UMeleeMinionAnim() :
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsInAir(false),
	m_bIsAttacking(false)
{
	loadMontages();

}

void UMeleeMinionAnim::NativeUpdateAnimation(float DeltaSeconds)	
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!m_Owner.IsValid())
	{
		m_Owner = Cast<AMeleeMinion>(TryGetPawnOwner());
	}
	else
	{
		m_CurSpeed = m_Owner->GetCurSpeed();
		m_bIsIdle = m_Owner->GetIsIdle();
		m_bIsWalking = m_Owner->GetIsWalking();
		m_bIsInAir = m_Owner->GetIsInAir();
		m_bIsAttacking = m_Owner->GetIsAttacking();
	}
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
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AMBP_NormalAttack0.AMBP_NormalAttack0'"));

	if (normalAttackMontage0.Succeeded())
	{
		m_AttackMontages.Add(normalAttackMontage0.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage1
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AMBP_NormalAttack1.AMBP_NormalAttack1'"));

	if (normalAttackMontage1.Succeeded())
	{
		m_AttackMontages.Add(normalAttackMontage1.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> criticalAttack
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AMBP_CriticalAttack.AMBP_CriticalAttack'"));

	if (criticalAttack.Succeeded())
	{
		m_AttackMontages.Add(criticalAttack.Object);
	}
}