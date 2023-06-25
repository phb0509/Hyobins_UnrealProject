// Copyright Epic Games, Inc. All Rights Reserved.


#include "HyobinsProjectGameModeBase.h"
#include "HyobinsProject.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerController.h"

AHyobinsProjectGameModeBase::AHyobinsProjectGameModeBase()
{
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AHyobinsProjectGameModeBase::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Call the GameModeBase::PostLogin!"));
}
