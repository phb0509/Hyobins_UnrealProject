// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"
#include "SubSystems/DebugManager.h"
#include "SubSystems/UIManager.h"
#include "Monster/Monster.h"

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
	check(m_ActorPool != nullptr);
	
	CreateMinions();
	
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsys tem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->ReserveMonsterHPBars();
}

void AFirstLevel::CreateMinions() const
{
	m_ActorPool->CreateActorPool(m_MonsterClass, 3);
	
	//m_ActorPool->CreateActorPool(m_TestMonsterClass,25);

	// for (int i = 0; i < 5; ++i)
	// {
	// 	for (int j = 0; j < 5; ++j)
	// 	{
	// 		m_ActorPool->SpawnActor<AActor>(m_TestMonsterClass, {i * 50.0f, 0.0f, j * 50.0f});
	// 	}
	// }
}

void AFirstLevel::SpawnMinion()
{
	const FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f},
		{0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };

	for (int i = 0; i < m_SpawnCount; ++i)
	{
		AActor* spawnedMonster = m_ActorPool->SpawnActor<AActor>(m_MonsterClass, positions[i]);
	}

	// for (int i = 0; i < 20; ++i)
	// {
	// 	for (int j = 0; j < 20; ++j)
	// 	{
	// 		AMonster* monster = Cast<AMonster>(m_ActorPool->SpawnActor<AActor>(m_MonsterClass, {i * 100.0f, j * 100.0f, 20.0f}));
	// 		monster->Pause();
	// 		monster->GetMesh()->SetHiddenInGame(true);
	// 	}
	// }
}

void AFirstLevel::ToggleDebugMode()
{
	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	check(debugManager != nullptr);

	debugManager->ToggleDebugMode();
}


void AFirstLevel::SpawnBoss()
{
	AActorPool* actorPool = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>()->GetActorPool();
	check(actorPool != nullptr);
	
	actorPool->CreateActorPool(m_LichKingClass,1);
	
	const FVector position = {0.0f, 0.0f, 0.0f};
	actorPool->SpawnActor<AActor>(m_LichKingClass, position);
}



void AFirstLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
  
	m_ActorPool->ClearActorPool();

	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->EmptyMonsterHPBars();
}


