// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimInstanceBase.h"


UAnimInstanceBase::UAnimInstanceBase() 
{
}

void UAnimInstanceBase::PlayMontage(const FName& montageName, float inPlayRate)
{
	if (m_Montages.Contains(montageName))
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("Montage isn't Valid"));
		Montage_Play(m_Montages[montageName], inPlayRate);
	}

}

void UAnimInstanceBase::JumpToMontageSection(const FName& montageName, int32 newSection)
{
	if (m_Montages.Contains(montageName))
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("Montage isn't Valid"));

		FString section = FString::FromInt(newSection);
		Montage_JumpToSection(*section, m_Montages[montageName]);
	}
}

UAnimMontage* UAnimInstanceBase::GetMontage(const FName& montageName)
{
	if (m_Montages.Contains(montageName))
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("Montage isn't Valid"));
		return m_Montages[montageName];
	}

	return nullptr;
}

void UAnimInstanceBase::AnimNotify_Pause()
{
	if (IsValid(GetCurrentActiveMontage()))
	{
		auto montage = GetCurrentActiveMontage();
		Montage_Pause(montage);
	}
}

void UAnimInstanceBase::AnimNotify_EndedDeath() const
{
	OnEndedDeath.Broadcast();
}
