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

	if (!m_AMainPlayer.IsValid())
	{
		m_AMainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	else
	{
		m_CurSpeed = m_AMainPlayer->GetVelocity().Size();
		m_bIsIdle = m_AMainPlayer->GetIsIdle();
		m_bIsPressingShift = m_AMainPlayer->GetIsPressingShift();
		m_bIsCombated = m_AMainPlayer->GetIsCombat();
		m_bIsWalking = m_AMainPlayer->GetIsWalking();
		m_bIsRunning = m_AMainPlayer->GetIsRunning();
		m_bIsInAir = m_AMainPlayer->GetIsInAir();
		m_bIsAttacking = m_AMainPlayer->GetIsAttacking();
		m_bIsHit = m_AMainPlayer->GetIsHit();
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
