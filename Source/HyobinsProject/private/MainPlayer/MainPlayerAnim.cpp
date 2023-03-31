// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim() :
	m_bIsPressingShift(false),
	m_bIsCombated(true)
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
	}
}

void UMainPlayerAnim::PlayNormalAttackMontage()
{
	Montage_Play(m_NormalAttackMontage, 1.2f);
}

void UMainPlayerAnim::JumpToNormalAttackMontageSection(int32 newSection)
{
	Montage_JumpToSection(GetNormalAttackMontageSectionName(newSection), m_NormalAttackMontage);
}

void UMainPlayerAnim::AnimNotify_checkNormalAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnNormalAttackHitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_checkNextNormalAttack()
{
	OnNextNormalAttackCheck.Broadcast();
}

FName UMainPlayerAnim::GetNormalAttackMontageSectionName(int32 section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), section));
}
