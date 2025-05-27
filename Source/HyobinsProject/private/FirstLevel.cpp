// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"
#include "SubSystems/DebugManager.h"


AFirstLevel::	AFirstLevel() :
	m_SpawnTimerTime_MeleeMinion(2.0f),
	m_SpawnCount(1)
{
	
}

void AFirstLevel::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AFirstLevel :: BeginPlay"));
	
	m_ActorPool = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>()->GetActorPool();
	
	CreateMinions();
}

void AFirstLevel::CreateMinions() const
{
	m_ActorPool->CreateActorPool(m_MonsterClass,8);
}

void AFirstLevel::SpawnMinion()
{
	const FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f},
		{0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };

	for (int i = 0; i < m_SpawnCount; ++i)
	{
		m_ActorPool->SpawnActor<AActor>(m_MonsterClass, positions[i]);
	}
}

void AFirstLevel::ToggleDebugMode()
{
	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	if (debugManager != nullptr)
	{
		debugManager->ToggleDebugMode();
	}
}


void AFirstLevel::SpawnBoss()
{
	// const FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f},
	// 	{0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };
	//
	// //AHyobinsProjectGameModeBase* gameModeBase = Cast<AHyobinsProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	//
	// for (int i = 0; i < 1; ++i)
	// {
	// 	TWeakObjectPtr<AActor> spawnedActor = m_ActorPool->SpawnActor(m_MonsterClass, positions[i]);
	//
	// 	// if (spawnedActor.IsValid() && gameModeBase != nullptr)
	// 	// {
	// 	// 	ACharacterBase* minion = Cast<ACharacterBase>(spawnedActor.Get());
	// 	// 	minion->OnDeath.AddUObject(gameModeBase, &AHyobinsProjectGameModeBase::CountMinionDeath);
	// 	// }
	// }

	AActorPool* actorPool = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>()->GetActorPool();
	actorPool->CreateActorPool(m_LichKingClass,1);
	
	const FVector position = {0.0f, 0.0f, 0.0f};
	actorPool->SpawnActor<AActor>(m_LichKingClass, position);
}



void AFirstLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
  
	m_ActorPool->ClearActorPool();
}


