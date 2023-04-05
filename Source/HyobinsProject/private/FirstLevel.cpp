// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "Monster/Minions/Melee/MeleeMinion.h"

AFirstLevel::AFirstLevel() :
	m_SpawnTimerTime_MeleeMinion(2.0f)
{
	UE_LOG(LogTemp, Warning, TEXT("FirstBase :: Constructor"));
}

void AFirstLevel::BeginPlay()
{
	Super::BeginPlay();


	GetWorldTimerManager().SetTimer(m_SpawnTimerHandle_MeleeMinion, this, &AFirstLevel::spawn, m_SpawnTimerTime_MeleeMinion, true);
}

void AFirstLevel::spawn()
{
	UE_LOG(LogTemp, Warning, TEXT("FirstBase :: Spawn"));
	float x = FMath::RandRange(0, 500);
	float y = FMath::RandRange(0, 500);
	
	classType = AMeleeMinion::StaticClass();
	const FVector location = { x,y,10 };
	const FRotator rotation = { 0,0,0 };

	if (classType != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("FirstBase :: classType is Not Null"));

		UWorld* const World = GetWorld();

		if (World)
		{
			UE_LOG(LogTemp, Warning, TEXT("FirstBase :: World Is Not Null"));
			GetWorld()->SpawnActor<AMeleeMinion>(classType, location, rotation);

			GetWorldTimerManager().SetTimer(m_SpawnTimerHandle_MeleeMinion, this, &AFirstLevel::spawn, m_SpawnTimerTime_MeleeMinion, true);
		}
	}
	
}