// Copyright Epic Games, Inc. All Rights Reserved.


#include "HyobinsProjectGameModeBase.h"
#include "HyobinsProject.h"
#include "MainPlayer/MainPlayer.h"

AHyobinsProjectGameModeBase::AHyobinsProjectGameModeBase()
{
	DefaultPawnClass = AMainPlayer::StaticClass();
	//PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AHyobinsProjectGameModeBase::PostLogin(APlayerController* newPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(newPlayer);
	UE_LOG(LogTemp, Warning, TEXT("PostLogin End"));
}
