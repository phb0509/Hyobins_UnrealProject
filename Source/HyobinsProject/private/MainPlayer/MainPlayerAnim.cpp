// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim():
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsPressingShift(false),
	m_bIsCombated(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsInAir(false),
	m_bIsAttacking(false),
	m_bIsHit(false)
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> attackMontage
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/AMBP_ComboAttack1.AMBP_ComboAttack1'"));

	if (attackMontage.Succeeded())
	{
		m_NormalAttackMontage = attackMontage.Object;
	}
}

void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!m_Owner.IsValid())
	{
		m_Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	else
	{
		m_CurSpeed = m_Owner->GetCurSpeed();
		m_bIsIdle = m_Owner->GetIsIdle();
		m_bIsPressingShift = m_Owner->GetIsPressingShift();
		m_bIsCombated = m_Owner->GetIsCombat();
		m_bIsWalking = m_Owner->GetIsWalking();
		m_bIsRunning = m_Owner->GetIsRunning();
		m_bIsInAir = m_Owner->GetIsInAir();
		m_bIsAttacking = m_Owner->GetIsAttacking();
		m_bIsHit = m_Owner->GetIsHit();
	}

	GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Red, FString::Printf(TEXT("Is Attacking??? : %d"), m_bIsAttacking));
}

void UMainPlayerAnim::PlayNormalAttackMontage()
{
	Montage_Play(m_NormalAttackMontage, 1.0f);
}

void UMainPlayerAnim::JumpToNormalAttackMontageSection(int32 newSection)
{
	Montage_JumpToSection(GetNormalAttackMontageSectionName(newSection), m_NormalAttackMontage);
}

void UMainPlayerAnim::AnimNotify_checkAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnAttackHitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_checkNextAttack()
{
	OnNextAttackCheck.Broadcast();
}

FName UMainPlayerAnim::GetNormalAttackMontageSectionName(int32 section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), section));
}
