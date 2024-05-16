// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool/ActorPool.h"
#include "ActorPool/PoolableActor.h"
#include "Kismet/GameplayStatics.h"

AActorPool::AActorPool() :
	m_DefaultSpawnCount(4)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorPool::CreateActorPool(TSubclassOf<AActor> classType, int actorCount)
{
	checkf(IsValid(classType), TEXT("ClassTypes doesn't inherit from Actor"));

	if (m_ActorPool.Contains(classType))
	{
		if (m_ActorPool[classType].actors.Num() >= actorCount)
		{
			return;
		}
		else
		{
			actorCount -= m_ActorPool[classType].actors.Num();
		}
	}
	else
	{
		FActors actorPool;
		m_ActorPool.Add(classType, actorPool);
	}

	for (int i = 0; i < actorCount; ++i)
	{
		const FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }); // Rotation, Location.
		AActor* const spawnedActor = GetWorld()->SpawnActor<AActor>(classType, spawnTransform);
		checkf(spawnedActor->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()), TEXT("SpawnedActors don't inherit interfaces."));

		IPoolableActor* const castedSpawnedActor = Cast<IPoolableActor>(spawnedActor);
		castedSpawnedActor->Initialize();
		castedSpawnedActor->DeActivate();

		m_ActorPool[classType].actors.Add(spawnedActor);
	}
}

void AActorPool::CreateBlueprintActorPool(const FName& path, int actorCount)
{
	const UObject* const loadedActor = StaticLoadObject(UObject::StaticClass(), nullptr, *path.ToString());
	checkf(IsValid(loadedActor), TEXT("BlueprintFilePath is Not Valid"));

	const UBlueprint* const blueprintActor = Cast<UBlueprint>(loadedActor);
	checkf(IsValid(blueprintActor), TEXT("Failed to Cast to Blueprint"));

	const TSubclassOf<AActor> classType = (UClass*)blueprintActor->GeneratedClass;
	checkf(IsValid(blueprintActor), TEXT("Failed to Cast to Actor"));

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
		checkf(spawnedActor->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()), TEXT("SpawnedActors don't inherit IPoolableActorInterfaces."));

		IPoolableActor* const castedSpawnedActor = Cast<IPoolableActor>(spawnedActor);
		castedSpawnedActor->Initialize();
		castedSpawnedActor->DeActivate();

		m_BlueprintActorPool[classType].actors.Add(spawnedActor);
	}
}

TWeakObjectPtr<AActor> AActorPool::SpawnActor(TSubclassOf<AActor> classType, const FVector& spawnLocation)
{
	checkf(IsValid(classType), TEXT("ClassTypes doesn't inherit from Actor"));
	
	if (m_ActorPool.Contains(classType) == false)
	{
		CreateActorPool(classType, m_DefaultSpawnCount);
		UE_LOG(LogTemp, Warning, TEXT("A new actor pool was created because no actor pool was created for the requested actor."));
	}

	TWeakObjectPtr<AActor> actor = nullptr;

	for (TWeakObjectPtr<AActor> poolableActor : m_ActorPool[classType].actors)
	{
		IPoolableActor* castedPoolableActor = Cast<IPoolableActor>(poolableActor);
		checkf(castedPoolableActor != nullptr, TEXT("Failed to Cast to IPoolableActor"));

		if (castedPoolableActor->GetIsActivated()) continue;

		actor = poolableActor;
		actor->SetActorLocation(spawnLocation);
		castedPoolableActor->Activate();
		break;
	}

	if (actor == nullptr)
	{
		CreateActorPool(classType, m_ActorPool[classType].actors.Num() * 2);
		SpawnActor(classType);
		return nullptr;
	}

	return actor;
}

TWeakObjectPtr<AActor> AActorPool::SpawnBlueprintActor(const FName& path, const FVector& spawnLocation)
{
	UObject* const loadedActor = StaticLoadObject(UObject::StaticClass(), nullptr, *path.ToString());
	checkf(IsValid(loadedActor), TEXT("BlueprintFilePath isn't Valid"));

	const UBlueprint* const blueprintActor = Cast<UBlueprint>(loadedActor);
	checkf(IsValid(blueprintActor), TEXT("Failed to Cast to Blueprint"));

	const TSubclassOf<AActor> classType = (UClass*)blueprintActor->GeneratedClass;
	checkf(IsValid(classType), TEXT("Failed to Cast to Actor"));

	if (m_BlueprintActorPool.Contains(classType) == false) // �̹� ����������� �ƿ� ���ٸ� (Ű����ü�� ���ٸ�)
	{
		CreateBlueprintActorPool(path, m_DefaultSpawnCount);
		UE_LOG(LogTemp, Warning, TEXT("A new actor pool was created because no actor pool was created for the requested actor."));
	}

	TWeakObjectPtr<AActor> actor = nullptr;

	for (TWeakObjectPtr<AActor> poolableActor : m_BlueprintActorPool[classType].actors)
	{
		IPoolableActor* const castedPoolableActor = Cast<IPoolableActor>(poolableActor);
		checkf(castedPoolableActor != nullptr, TEXT("Failed to Cast to IPoolableActor"));

		if (castedPoolableActor->GetIsActivated())
		{
			continue;
		}
		
		actor = poolableActor;
		actor->SetActorLocation(spawnLocation);
		castedPoolableActor->Activate();
		break;
	}

	if (actor == nullptr)
	{
		CreateBlueprintActorPool(path, m_BlueprintActorPool[classType].actors.Num() * 2);
		actor = SpawnBlueprintActor(path);
	}

	return actor;
}

void AActorPool::ClearActorPool()
{
	for (const auto& iter : m_ActorPool)
	{
		const TSubclassOf<AActor>& classType = iter.Key;
		const FActors& actors = m_ActorPool[classType];

		for (const auto& actor : actors.actors)
		{
			actor->Destroy();
		}
	}

	m_ActorPool.Empty();
}

void AActorPool::ClearBlueprintActorPool()
{
	for (const auto& iter : m_BlueprintActorPool)
	{
		const TSubclassOf<AActor>& classType = iter.Key;
		const FActors& actors = m_BlueprintActorPool[classType];

		for (const auto& actor : actors.actors)
		{
			actor->Destroy();
		}
	}

	m_BlueprintActorPool.Empty();
}