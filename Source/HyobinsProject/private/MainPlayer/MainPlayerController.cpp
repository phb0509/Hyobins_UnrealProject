// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"
#include "UI/System/EnvironmentSettings.h"
#include "SubSystems/UIManager.h"

AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1)),
	m_bIsOpenedEnvironmentSettings(false)
{
	UE_LOG(LogTemp, Warning, TEXT("MainPlayerController::Constructor"));
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_EnvironmentSettings = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->GetEnvironmentSettings();
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}

void AMainPlayerController::OpenEnvironmentSettingsState()
{
	if (!m_bIsOpenedEnvironmentSettings) // 창을 닫고 게임모드로 전환.
	{
		SetPause(true);
		SetShowMouseCursor(true);
		m_EnvironmentSettings->SetVisibility(ESlateVisibility::Visible);
	}
	
	m_bIsOpenedEnvironmentSettings = !m_bIsOpenedEnvironmentSettings;
}