// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/DebugManager.h"


UDebugManager::UDebugManager() :
	m_bIsDebugMode(true)
{
}

void UDebugManager::ToggleDebugMode()
{
	m_bIsDebugMode = !m_bIsDebugMode;

	if (m_bIsDebugMode)
	{
		GetWorld()->GetTimerManager().SetTimer(m_LogTimer,
		[this]()
			{
				OnDebugMode.Broadcast();
			},
		GetWorld()->DeltaTimeSeconds,
		true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(m_LogTimer);
	}
}

