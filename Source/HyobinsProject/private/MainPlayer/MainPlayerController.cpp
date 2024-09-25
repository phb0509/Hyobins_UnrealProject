// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"
#include "SubSystems/UIManager.h"

AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1))
{
	
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}

void AMainPlayerController::OpenEnvironmentSettingsState() const
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->OpenEnvironmentSettings();
}