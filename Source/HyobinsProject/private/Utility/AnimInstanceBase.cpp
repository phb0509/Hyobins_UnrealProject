// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimInstanceBase.h"
#include "Utility/CharacterBase.h"


UAnimInstanceBase::UAnimInstanceBase() :
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsInAir(false),
	m_bIsDeath(false)
{

}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!m_Owner.IsValid())
	{
		m_Owner = Cast<ACharacterBase>(TryGetPawnOwner());
	}
	else
	{
		m_CurSpeed = m_Owner->GetCurSpeed();
		m_bIsIdle = m_Owner->GetIsIdle();
		m_bIsWalking = m_Owner->GetIsWalking();
		m_bIsInAir = m_Owner->GetIsInAir();
		m_bIsDeath = m_Owner->GetIsDeath();
	}
}

void UAnimInstanceBase::PlayMontage(FName montageName, float inPlayRate)
{
	if (m_Montages.Contains(montageName))
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("??????????????"));

		Montage_Play(m_Montages[montageName], inPlayRate);
	}
}

void UAnimInstanceBase::JumpToMontageSection(FName montageName, int32 newSection)
{
	if (m_Montages.Contains(montageName))
	{
		checkf(IsValid(m_Montages[montageName]), TEXT(""));

		FString section = FString::FromInt(newSection);
		Montage_JumpToSection(*section, m_Montages[montageName]);
	}
}

void UAnimInstanceBase::AnimNotify_OnDeathMontageEnded()
{
	OnDeathMontageEnded.Broadcast();
}
