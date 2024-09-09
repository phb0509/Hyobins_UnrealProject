// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"


void UActorPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	m_ActorPool = GetWorld()->SpawnActor<AActorPool>();
}
