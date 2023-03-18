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
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttackMontage
	(TEXT("AnimMontage'/Game/MonsterAsset/Minion/AMBP_NormalAttack1.AMBP_NormalAttack1'"));

	if (normalAttackMontage.Succeeded())
	{
		m_NormalAttackMontage = normalAttackMontage.Object;
	}
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
	Montage_Play(m_NormalAttackMontage, 0.5f); // �������� ������ ���.
}

void UMeleeMinionAnim::AnimNotify_checkAttackHit() // ��Ƽ���� ���� �Լ�. ��Ÿ�������� ��Ƽ�����̸��� �����ϰ� �����ؾ��Ѵ�.
{
	OnAttackHitCheck.Broadcast();
}