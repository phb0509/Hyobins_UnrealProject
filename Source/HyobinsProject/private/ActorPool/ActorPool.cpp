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
		if (m_ActorPool[classType].actors.Num() < requestedCapacity)  // ���� ��������� ��û�����ͺ��� ���ٸ�
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
		const FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 10000.0f });
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
		}
	}
}

AActor* AActorPool::spawnActor(const TSubclassOf<AActor> classType, const FVector& spawnLocation)
{
	if(!classType->ImplementsInterface(UPoolableActor::StaticClass()))
	{
		return nullptr;
	}
	
	if (!m_ActorPool.Contains(classType)) // �̹� ����������� �ƿ� ���ٸ� (Ű����ü�� ���ٸ�)
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

	if (actor == nullptr) // ����Ǯ�� ��á�ٸ� (���� ���Ͱ� ���ٸ�)
	{
		CreateActorPool(classType, m_ActorPool[classType].actors.Num() * 2.0f); // �߰�����.
		actor = spawnActor(classType);
	}

	return actor;
}


void AActorPool::ClearActorPool()
{
	for (const auto& iter : m_ActorPool)
	{
		const TSubclassOf<AActor>& classType = iter.Key;
		const FActors& blueprintActorPoolActors = m_ActorPool[classType];

		for (AActor* actor : blueprintActorPoolActors.actors)
		{
			if (actor != nullptr)
			{
				actor->Destroy();
			}
		}
	}

	m_ActorPool.Empty();
}

