// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"


void UActorPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogTemp, Warning, TEXT("UActorPoolManager :: Initialize"));
}

void UActorPoolManager::Deinitialize()
{
	Super::Deinitialize();
	
	UE_LOG(LogTemp, Warning, TEXT("UActorPoolManager :: Deinitialize"));
}

AActorPool* UActorPoolManager::GetActorPool()
{
	if (m_ActorPool == nullptr)
	{
		m_ActorPool = GetWorld()->SpawnActor<AActorPool>();
	}

	return m_ActorPool;
}
