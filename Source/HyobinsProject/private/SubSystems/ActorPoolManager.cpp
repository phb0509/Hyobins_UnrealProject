// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"


AActorPool* UActorPoolManager::GetActorPool()
{
	if (m_ActorPool == nullptr)
	{
		m_ActorPool = GetWorld()->SpawnActor<AActorPool>();
	}

	return m_ActorPool;
}
