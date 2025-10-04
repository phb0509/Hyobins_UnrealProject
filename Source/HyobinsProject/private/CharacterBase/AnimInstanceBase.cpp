

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

void UAnimInstanceBase::Exec_OnMontageStarted(UAnimMontage* montage) // 호출됐다는거 자체가 몽타주가 유효한거라 유효성 체크x.
{
	const FName montageName = montage->GetFName();
	
	if (m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.ExecuteIfBound();
	}
}

void UAnimInstanceBase::Exec_OnMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
	const FName montageName = montage->GetFName();
	m_bIsLastMontagePlayInterrupted = bInterrupted;
	
	if (m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageAllEnded.ExecuteIfBound();
		
		if (bInterrupted)
		{
			m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageInterruptedEnded.ExecuteIfBound();
		}
		else
		{
			m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageNotInterruptedEnded.ExecuteIfBound();
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

void UAnimInstanceBase::JumpToMontageSectionByIndex(const UAnimMontage* montage, int32 newSection)
{
	check(montage != nullptr);
	
	const FString section = FString::FromInt(newSection);
	Montage_JumpToSection(*section, montage);
}

void UAnimInstanceBase::JumpToMontageSectionByName(const UAnimMontage* montage, FName newSection)
{
	check(montage != nullptr);

	Montage_JumpToSection(newSection, montage);
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

float UAnimInstanceBase::GetMontagePlayTime(const UAnimMontage* montage) 
{
	check(montage != nullptr);
	
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
