// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"
#include "UI/System/EnvironmentSettings.h"
#include "SubSystems/UIManager.h"
#include "UI/System/Combo.h"

AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1)),
	m_bIsOpenedEnvironmentSettings(false)
{
	
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_EnvironmentSettings = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->GetEnvironmentSettingsWidjet();
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}

void AMainPlayerController::OpenEnvironmentSettingsState()
{
	if (!m_bIsOpenedEnvironmentSettings) // 환경설정창 오픈.
	{
		SetPause(true);
		SetShowMouseCursor(true);
		m_EnvironmentSettings->SetVisibility(ESlateVisibility::Visible);
	}
	
	m_bIsOpenedEnvironmentSettings = !m_bIsOpenedEnvironmentSettings;
}