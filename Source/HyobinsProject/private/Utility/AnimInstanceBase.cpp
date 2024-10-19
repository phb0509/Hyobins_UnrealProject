

#include "Utility/AnimInstanceBase.h"


UAnimInstanceBase::UAnimInstanceBase() 
{
}

void UAnimInstanceBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	OnMontageStarted.AddDynamic(this, &UAnimInstanceBase::exec_OnMontageStarted);
	OnMontageEnded.AddDynamic(this, &UAnimInstanceBase::exec_OnMontageEnded);
}

void UAnimInstanceBase::exec_OnMontageStarted(UAnimMontage* Montage)
{
	const FName montageName = Montage->GetFName();
	execBindedFunc_OnMontageStarted(montageName);
}

void UAnimInstanceBase::exec_OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	const FName montageName = Montage->GetFName();

	if (bInterrupted)
	{
		execBindedFunc_OnMontageInterruptedEnded(montageName);
	}
	else
	{
		execBindedFunc_OnMontageEnded(montageName);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Ended Montage : %s"), *montageName.ToString());
}

void UAnimInstanceBase::execBindedFunc_OnMontageStarted(const FName& montageName)
{
	if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		return;
	}
	
	const bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.ExecuteIfBound();
}

void UAnimInstanceBase::execBindedFunc_OnMontageEnded(const FName& montageName)
{
	if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		return;
	}
	
	const bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageNotInterruptedEnded.ExecuteIfBound();
}

void UAnimInstanceBase::execBindedFunc_OnMontageInterruptedEnded(const FName& montageName)
{
	if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		return;
	}
	
	const bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageInterruptedEnded.ExecuteIfBound();
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

		const FString section = FString::FromInt(newSection);
		Montage_JumpToSection(*section, m_Montages[montageName]);
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
		checkf(IsValid(m_Montages[montageName]), TEXT("Montage isn't Valid"));
		
		return m_Montages[montageName];
	}

	return nullptr;
}

void UAnimInstanceBase::AnimNotify_Pause()
{
	const UAnimMontage* montage = GetCurrentActiveMontage();
	
	Montage_Pause(montage);
}

void UAnimInstanceBase::AnimNotify_End_Death() const
{
	End_Death.Broadcast();
}
