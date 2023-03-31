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

	static ConstructorHelpers::FObjectFinder <UAnimMontage> criticalAttackMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AMBP_CriticalAttack.AMBP_CriticalAttack'"));

	if (criticalAttackMontage.Succeeded())
	{
		m_AttackMontages.Add(criticalAttackMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> onHitMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AMBP_HitReact_Front.AMBP_HitReact_Front'"));

	if (onHitMontage.Succeeded())
	{
		m_OnHitMontages.Add(onHitMontage.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> deathMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/Death.Death'"));

	if (deathMontage.Succeeded())
	{
		m_DeathMontages.Add(deathMontage.Object);
	}
}