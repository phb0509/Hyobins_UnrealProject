// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPersistantLevel.h"
#include "Kismet/GameplayStatics.h"

AMainPersistantLevel::AMainPersistantLevel()
{
}

void AMainPersistantLevel::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ALoadingLevel :: BeginPlay"));
	UGameplayStatics::OpenLevel(this, "LoadingLevel");
}
