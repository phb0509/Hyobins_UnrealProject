// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"

#include "SubSystems/BattleManager.h"
#include "SubSystems/UIManager.h"


AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1))
{
	
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	UBattleManager* battleManager = GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
	
	battleManager->OnPlayerAttack.AddUObject(uiManager, &UUIManager::UpdateComboCount);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMainPlayerController::OpenEnvironmentSettingsState() const
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->OpenEnvironmentSettings();
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}
