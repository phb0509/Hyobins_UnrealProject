// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystems/UIManager.h"

AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1)),
	m_bIsOpenedEnvironmentSettings(false)
{
	UE_LOG(LogTemp, Warning, TEXT("MainPlayerController::Constructor"));
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}

void AMainPlayerController::ChangeEnvironmentSettingsState()
{
	if (m_bIsOpenedEnvironmentSettings)
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->HideEnvironmentSettings();
		SetShowMouseCursor(false);
	}
	else
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ShowEnvironmentSettings();
		SetShowMouseCursor(true);
	}
	
	m_bIsOpenedEnvironmentSettings = !m_bIsOpenedEnvironmentSettings;
	
	//SetPause(true);
	//SetInputMode(m_UIInputMode);
	//UGameplayStatics::SetGamePaused(GetWorld(),true);
}