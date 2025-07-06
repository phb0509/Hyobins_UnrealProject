

#include "CharacterBase/AnimInstanceBase.h"


UAnimInstanceBase::UAnimInstanceBase() :
	m_bIsLastMontagePlayInterrupted(false)
{
}

void UAnimInstanceBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	OnMontageStarted.AddDynamic(this, &UAnimInstanceBase::Exec_OnMontageStarted);
	OnMontageEnded.AddDynamic(this, &UAnimInstanceBase::Exec_OnMontageEnded);
}

void UAnimInstanceBase::Exec_OnMontageStarted(UAnimMontage* Montage)
{
	const FName montageName = Montage->GetFName();
	
	if (m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		const bool bIsBounded = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.ExecuteIfBound();
	}
}

void UAnimInstanceBase::Exec_OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	const FName montageName = Montage->GetFName();
	m_bIsLastMontagePlayInterrupted = bInterrupted;
	
	if (m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		const bool allEndedBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageAllEnded.ExecuteIfBound();
		
		if (bInterrupted)
		{
			const bool bIsInterruptedEndedBounded = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageInterruptedEnded.ExecuteIfBound();
		}
		else
		{
			const bool bIsNotInterruptedEndedBounded = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageNotInterruptedEnded.ExecuteIfBound();
		}
	}
}

void UAnimInstanceBase::PlayMontage(const FName& montageName, float inPlayRate)
{
	if (m_Montages.Contains(montageName))
	{
		if(IsValid(m_Montages[montageName]))
		{
			Montage_Play(m_Montages[montageName].Get(), inPlayRate);
		}
	}
}

void UAnimInstanceBase::JumpToMontageSectionByIndex(const FName& montageName, int32 newSection)
{
	if (m_Montages.Contains(montageName))
	{
		if(IsValid(m_Montages[montageName]))
		{
			const FString section = FString::FromInt(newSection);
			Montage_JumpToSection(*section, m_Montages[montageName].Get());
		}
	}
}

void UAnimInstanceBase::JumpToMontageSectionByName(const FName& montageName, FName newSection)
{
	if (m_Montages.Contains(montageName))
	{
		if(IsValid(m_Montages[montageName]))
		{
			Montage_JumpToSection(newSection, m_Montages[montageName].Get());
		}
	}
}

bool UAnimInstanceBase::IsCurrentMontage(const FName& montageName)
{
	UAnimMontage* curMontage = GetCurrentActiveMontage();
	UAnimMontage* targetMontage = GetMontage(montageName);

	if ((curMontage != nullptr && targetMontage != nullptr) && curMontage == targetMontage)
	{
		return true;
	}
	
	return false;
}

FName UAnimInstanceBase::GetCurrentMontageName() const
{
	const UAnimMontage* montage = GetCurrentActiveMontage();

	return montage == nullptr ? "" : montage->GetFName();
}

float UAnimInstanceBase::GetMontagePlayTime(const FName& montageName)
{
	const UAnimMontage* montage = GetMontage(montageName);
	const int32 frameCount = montage->GetNumberOfSampledKeys();
	
	return montage->GetTimeAtFrame(frameCount) / montage->RateScale;
}

UAnimMontage* UAnimInstanceBase::GetMontage(const FName& montageName)
{
	if (m_Montages.Contains(montageName))
	{
		if(IsValid(m_Montages[montageName]))
		{
			return m_Montages[montageName].Get();
		}
	}

	return nullptr;
}

void UAnimInstanceBase::AnimNotify_Pause()
{
	const UAnimMontage* montage = GetCurrentActiveMontage();
	
	if (montage != nullptr)
	{
		Montage_Pause(montage);
	}
}

void UAnimInstanceBase::AnimNotify_End_Death() const
{
	End_Death.Broadcast();
}
