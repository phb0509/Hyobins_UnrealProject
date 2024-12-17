// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"

#include "HyobinsProjectGameModeBase.h"
#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"
#include "Utility/CharacterBase.h"


AFirstLevel::AFirstLevel() :
	m_SpawnTimerTime_MeleeMinion(2.0f)
{
}

void AFirstLevel::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AFirstLevel :: BeginPlay"));

	UWorld* world = GetWorld();
	UGameInstance* gameinstance = GetWorld()->GetGameInstance();
	UActorPoolManager* poolManager = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>();
	
	m_ActorPool = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>()->GetActorPool();
	
	CreateMinions();
}

void AFirstLevel::CreateMinions() const
{
	m_ActorPool->CreateActorPool(m_MonsterClass,3);
}

void AFirstLevel::Spawn()
{
	const FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f},
		{0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };

	AHyobinsProjectGameModeBase* gameModeBase = Cast<AHyobinsProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	
	for (int i = 0; i < 3; ++i)
	{
		TWeakObjectPtr<AActor> spawnedActor = m_ActorPool->SpawnActor(m_MonsterClass, positions[i]);

		if (spawnedActor.IsValid() && gameModeBase != nullptr)
		{
			ACharacterBase* minion = Cast<ACharacterBase>(spawnedActor.Get());
			minion->OnDeath.AddUObject(gameModeBase, &AHyobinsProjectGameModeBase::CountMinionDeath);
		}
	}
}

void AFirstLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_ActorPool->ClearActorPool();
}


