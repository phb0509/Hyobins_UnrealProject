// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Level/LoadingScreen.h"

void ULoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("ULoadingScreen :: NativeConstruct"));
}

void ULoadingScreen::SetLoadingPercent(float percent)
{
}
