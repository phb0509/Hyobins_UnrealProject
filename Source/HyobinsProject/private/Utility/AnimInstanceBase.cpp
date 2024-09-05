

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

void UAnimInstanceBase::ExecBindedFunc_OnMontageStarted(const FName& montageName)
{
	if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		return;
	}
	
	bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.ExecuteIfBound();
	if (!bIsBound)
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("The function is not bound"));
	}
}

void UAnimInstanceBase::ExecBindedFunc_OnMontageEnded(const FName& montageName)
{
	if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
	{
		return;
	}
	
	bool bIsBound = m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageEnded.ExecuteIfBound();
	if (!bIsBound)
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("The function is not bound"));
	}
}

void UAnimInstanceBase::Exec_OnMontageStarted(UAnimMontage* Montage) // 몽타주 start할때마다 호출.
{
	FName montageName = Montage->GetFName(); // enum 문자열이랑 동일.
	ExecBindedFunc_OnMontageStarted(montageName);

	//UE_LOG(LogTemp, Warning, TEXT("Started Montage : %s"), *montageName.ToString());
}

void UAnimInstanceBase::Exec_OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) // 몽타주 End할때마다 호출.
{
	FName montageName = Montage->GetFName(); 
	ExecBindedFunc_OnMontageEnded(montageName);
	
	//UE_LOG(LogTemp, Warning, TEXT("Ended Montage : %s"), *montageName.ToString());
}

void UAnimInstanceBase::AnimNotify_Pause()
{
	if (IsValid(GetCurrentActiveMontage()))
	{
		UAnimMontage* montage = GetCurrentActiveMontage();
		Montage_Pause(montage);
	}
}

void UAnimInstanceBase::AnimNotify_End_Death() const
{
	End_Death.Broadcast();
}

void UAnimInstanceBase::AnimNotify_End_Down() 
{
	PlayMontage(TEXT("GetUp")); // 땅에 닿았을 때의 몽타주 재생.
}

void UAnimInstanceBase::AnimNotify_End_GetUp()
{
	End_GetUp.Broadcast();
}
