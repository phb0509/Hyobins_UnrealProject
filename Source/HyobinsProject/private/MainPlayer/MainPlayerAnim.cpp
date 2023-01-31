// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim():
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsPressingShift(false),
	m_bIsCombated(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsInAir(false)
{
	
}

void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!m_AMainPlayer.IsValid())
	{
		m_AMainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	else
	{
		m_CurSpeed = m_AMainPlayer->GetVelocity().Size();
		m_bIsIdle = m_AMainPlayer->GetIsIdle();
		m_bIsPressingShift = m_AMainPlayer->GetIsPressingShift();
		m_bIsCombated = m_AMainPlayer->GetIsCombat();
		m_bIsWalking = m_AMainPlayer->GetIsWalking();
		m_bIsRunning = m_AMainPlayer->GetIsRunning();
		m_bIsInAir = m_AMainPlayer->GetIsInAir();
	}

	//GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Red, FString::Printf(TEXT("MainPlayerAnimInstance_CurState : %d"), m_CurState));
}
