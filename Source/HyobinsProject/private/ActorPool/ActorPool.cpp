// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool/ActorPool.h"
#include "Interfaces//PoolableActor.h"


AActorPool::AActorPool() :
	m_DefaultSpawnCount(4)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorPool::CreateActorPool(const TSubclassOf<AActor> classType, int requestedCapacity)
{
	if(!classType->ImplementsInterface(UPoolableActor::StaticClass()))
	{
		return;
	}
	
	if (m_ActorPool.Contains(classType)) 
	{
		if (m_ActorPool[classType].actors.Num() < requestedCapacity)  // 현재 만들어진게 요청받은것보다 적다면
		{
			requestedCapacity -= m_ActorPool[classType].actors.Num();
		}
		else
		{
			return;
		}
	}
	else
	{
		const FActors actorPool;
		m_ActorPool.Add(classType, actorPool);
	}

	for (int i = 0; i < requestedCapacity; ++i)
	{
		const FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 100000.0f });
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(classType, spawnTransform);

		if (spawnedActor != nullptr)
		{
			IPoolableActor* poolableActor = Cast<IPoolableActor>(spawnedActor);

			if (poolableActor != nullptr)
			{
				poolableActor->Initialize();
				poolableActor->Deactivate();
				
				m_ActorPool[classType].actors.Add(spawnedActor);
			}
			else
			{
				spawnedActor->Destroy();
			}
		}
	}
}

AActor* AActorPool::spawnActor(const TSubclassOf<AActor> classType, const FVector& spawnLocation)
{
	if(!classType->ImplementsInterface(UPoolableActor::StaticClass()))
	{
		return nullptr;
	}
	
	if (!m_ActorPool.Contains(classType)) // 이미 만들어진적이 아예 없다면 (키값자체가 없다면)
	{
		CreateActorPool(classType, m_DefaultSpawnCount);
	}

	AActor* actor = nullptr;

	for (AActor* poolableActor : m_ActorPool[classType].actors)
	{
		if (poolableActor != nullptr)
		{
			IPoolableActor* castedPoolableActor = Cast<IPoolableActor>(poolableActor);
		
			if (castedPoolableActor == nullptr ||
				castedPoolableActor->IsActive())
			{
				continue;
			}
			
			actor = poolableActor;
			actor->SetActorLocation(spawnLocation);
			castedPoolableActor->Activate();
			
			break;
		}
	}

	if (actor == nullptr) // 액터풀이 꽉찼다면 (꺼낼 액터가 없다면)
	{
		CreateActorPool(classType, m_ActorPool[classType].actors.Num() * 2.0f); // 추가생성.
		actor = spawnActor(classType);
	}

	return actor;
}


void AActorPool::ClearActorPool()
{
	for (const auto& iter : m_ActorPool)
	{
		const TSubclassOf<AActor>& classType = iter.Key;
		const FActors& actorPool = m_ActorPool[classType];

		for (AActor* actor : actorPool.actors)
		{
			if (IsValid(actor))
			{
				actor->Destroy();
			}
		}
	}

	m_ActorPool.Empty();
}

