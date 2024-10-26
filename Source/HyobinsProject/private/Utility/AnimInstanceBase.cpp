

#include "Utility/AnimInstanceBase.h"


UAnimInstanceBase::UAnimInstanceBase() 
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

	if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		return;
	}
	
	const bool bIsBounded = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.ExecuteIfBound();
}

void UAnimInstanceBase::Exec_OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	const FName montageName = Montage->GetFName();

	if (bInterrupted)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			return;
		}
	
		const bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageInterruptedEnded.ExecuteIfBound();
	}
	else
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			return;
		}
        	
		const bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageNotInterruptedEnded.ExecuteIfBound();
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Ended Montage : %s"), *montageName.ToString());
}

void UAnimInstanceBase::PlayMontage(const FName& montageName, float inPlayRate)
{
	if (m_Montages.Contains(montageName))
	{
		if(IsValid(m_Montages[montageName]))
		{
			Montage_Play(m_Montages[montageName], inPlayRate);
		}
	}
}

void UAnimInstanceBase::JumpToMontageSection(const FName& montageName, int32 newSection)
{
	if (m_Montages.Contains(montageName))
	{
		if(IsValid(m_Montages[montageName]))
		{
			const FString section = FString::FromInt(newSection);
			Montage_JumpToSection(*section, m_Montages[montageName]);
		}
	}
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
			return m_Montages[montageName];
		}
	}

	return nullptr;
}

void UAnimInstanceBase::AnimNotify_Pause()
{
	const UAnimMontage* montage = GetCurrentActiveMontage();
	
	if (IsValid(montage))
	{
		Montage_Pause(montage);
	}
}

void UAnimInstanceBase::AnimNotify_End_Death() const
{
	End_Death.Broadcast();
}
