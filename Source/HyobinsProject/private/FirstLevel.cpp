// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"
#include "SubSystems/UIManager.h"


AFirstLevel::AFirstLevel() :
	m_SpawnTimerTime_MeleeMinion(2.0f)
{
}

void AFirstLevel::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AFirstLevel :: BeginPlay"));
	
	m_ActorPool = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>()->GetActorPool();
	Create();
}

void AFirstLevel::Create()
{
	m_ActorPool->CreateActorPool(m_MonsterClass,5);
}

void AFirstLevel::Spawn()
{
	FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f}, {0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };

	for (int i = 0; i < 1; ++i)
	{
		m_ActorPool->SpawnActor(m_MonsterClass, positions[i]);
	}
}

