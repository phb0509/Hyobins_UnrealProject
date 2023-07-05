// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimInstanceBase.h"
#include "Utility/CharacterBase.h"


UAnimInstanceBase::UAnimInstanceBase() 
{

}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*if (!m_Owner.IsValid())
	{
		m_Owner = Cast<ACharacterBase>(TryGetPawnOwner());
	}*/

}

void UAnimInstanceBase::PlayMontage(FName montageName, float inPlayRate)
{
	if (m_Montages.Contains(montageName))
	{
		checkf(IsValid(m_Montages[montageName]), TEXT("Montage isn't Valid"));
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
