// Copyright Epic Games, Inc. All Rights Reserved.


#include "HyobinsProjectGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/GameSession.h"
#include "MainPlayer/MainPlayerController.h"

AHyobinsProjectGameModeBase::AHyobinsProjectGameModeBase()
{
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AHyobinsProjectGameModeBase::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
	UE_LOG(LogTemp, Warning, TEXT("GameModeBase::PostLogin!"));
}

void AHyobinsProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("GameModeBase::BeginPlay!"));
	
	const TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Widget_SettingPopup.Widget_SettingPopup_C'"));

	
}
