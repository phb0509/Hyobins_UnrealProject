// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim():
	m_CurSpeed(0.0f),
	m_bIsCombated(true),
	m_bIsPressingShift(false),
	m_bIsWalking(false),
	m_bIsRunning(false)
{
	
}

void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!m_MainPlayer.IsValid())
	{
		m_MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	else
	{
		m_CurSpeed = m_MainPlayer->GetVelocity().Size();
		m_bIsCombated = m_MainPlayer->GetIsCombat();
		m_bIsPressingShift = m_MainPlayer->GetIsPressingShift();
	}

	//GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Red, FString::Printf(TEXT("MainPlayerAnimInstance_CurState : %d"), m_CurState));
}
