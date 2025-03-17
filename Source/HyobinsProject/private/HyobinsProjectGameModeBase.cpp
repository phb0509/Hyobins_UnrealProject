// Copyright Epic Games, Inc. All Rights Reserved.


#include "HyobinsProjectGameModeBase.h"
#include "MainPlayer/MainPlayerController.h"
#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"

AHyobinsProjectGameModeBase::AHyobinsProjectGameModeBase() :
	m_MinionDeathCount(1)
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
	
}

void AHyobinsProjectGameModeBase::CountMinionDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("AHyobinsProjectGameModeBase :: CountMinionDeath"));
	
	--m_MinionDeathCount;

	if (m_MinionDeathCount == 0)
	{
		spawnBossMonster();
	}
}

void AHyobinsProjectGameModeBase::spawnBossMonster()
{
	AActorPool* actorPool = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>()->GetActorPool();
	actorPool->CreateActorPool(m_LichKingClass,1);
	
	const FVector position = {0.0f, 0.0f, 0.0f};
	actorPool->SpawnActor<AActor>(m_LichKingClass, position);
}
