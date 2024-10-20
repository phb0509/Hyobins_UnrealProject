// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool/ActorPool.h"
#include "ActorPool/PoolableActor.h"


AActorPool::AActorPool() :
	m_DefaultSpawnCount(4)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorPool::CreateActorPool(const TSubclassOf<AActor> classType, int actorCount)
{
	checkf(classType->ImplementsInterface(UPoolableActor::StaticClass()), TEXT("SpawnedActors don't inherit IPoolableActorInterfaces."));
	
	if (m_BlueprintActorPool.Contains(classType)) // �̹� ��������� �ִٸ�
	{
		if (m_BlueprintActorPool[classType].actors.Num() >= actorCount) // ���� ��������� ��û�����ͺ��� ���ų� ���ٸ� -> ���� �����ʿ����.
		{
			return;
		}
		else // ���� ��������� ��û�����ͺ��� ���ٸ�
		{
			actorCount -= m_BlueprintActorPool[classType].actors.Num();
		}
	}
	else
	{
		const FActors actorPool;
		m_BlueprintActorPool.Add(classType, actorPool);
	}

	for (int i = 0; i < actorCount; ++i)
	{
		const FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 10000.0f });
		AActor* const spawnedActor = GetWorld()->SpawnActor<AActor>(classType, spawnTransform);

		IPoolableActor* const castedSpawnedActor = Cast<IPoolableActor>(spawnedActor);
		castedSpawnedActor->Initialize();
		castedSpawnedActor->DeActivate();

		m_BlueprintActorPool[classType].actors.Add(spawnedActor);
	}
}


TWeakObjectPtr<AActor> AActorPool::SpawnActor(const TSubclassOf<AActor> classType, const FVector& spawnLocation)
{
	checkf(classType->ImplementsInterface(UPoolableActor::StaticClass()), TEXT("SpawnedActors don't inherit IPoolableActorInterfaces."));

	if (m_BlueprintActorPool.Contains(classType) == false) // �̹� ����������� �ƿ� ���ٸ� (Ű����ü�� ���ٸ�)
	{
		CreateActorPool(classType, m_DefaultSpawnCount);
		UE_LOG(LogTemp, Warning, TEXT("A new actor pool was created because no actor pool was created for the requested actor."));
	}

	TWeakObjectPtr<AActor> actor = nullptr;

	for (TWeakObjectPtr<AActor> poolableActor : m_BlueprintActorPool[classType].actors)
	{
		IPoolableActor* const castedPoolableActor = Cast<IPoolableActor>(poolableActor);
		
		if (castedPoolableActor->GetIsActivated())
		{
			continue;
		}
		
		actor = poolableActor;
		actor->SetActorLocation(spawnLocation);
		castedPoolableActor->Activate();
		break;
	}

	if (!actor.IsValid())
	{
		CreateActorPool(classType, m_BlueprintActorPool[classType].actors.Num() * 2);
		actor = SpawnActor(classType);
	}

	return actor;
}


void AActorPool::ClearBlueprintActorPool()
{
	for (const auto& iter : m_BlueprintActorPool)
	{
		const TSubclassOf<AActor>& classType = iter.Key;
		const FActors& blueprintActorPoolActors = m_BlueprintActorPool[classType];

		for (const auto& actor : blueprintActorPoolActors.actors)
		{
			actor->Destroy();
		}
	}

	m_BlueprintActorPool.Empty();
}

