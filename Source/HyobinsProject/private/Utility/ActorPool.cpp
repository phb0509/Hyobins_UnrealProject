// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ActorPool.h"
#include "Utility/PoolableActor.h"
#include "Kismet/GameplayStatics.h"

AActorPool::AActorPool() :
	m_DefaultSpawnCount(10)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorPool::CreateActorPool(TSubclassOf<AActor> classType, int actorCount)
{
	checkf(IsValid(classType), TEXT("ClassType doesn't inherit from Actor"));

	if (m_ActorPools.Contains(classType)) 
	{
		if (m_ActorPools[classType].actors.Num() >= actorCount)
		{
			return;
		}
		else 
		{
			actorCount -= m_ActorPools[classType].actors.Num();
		}
	}
	else
	{
		FActors actorPool;
		m_ActorPools.Add(classType, actorPool);
	}

	for (int i = 0; i < actorCount; ++i)
	{
		FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }); // Rotation, Location.
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(classType, spawnTransform);
		checkf(spawnedActor->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()), TEXT("SpawnedActor doesn't inherit interfaces."));

		IPoolableActor* castedSpawnedActor = Cast<IPoolableActor>(spawnedActor);
		castedSpawnedActor->DeActivate();

		m_ActorPools[classType].actors.Add(spawnedActor);
	}
}

void AActorPool::CreateBlueprintActorPool(FName path, int actorCount)
{
	UObject* loadedActor = StaticLoadObject(UObject::StaticClass(), nullptr, *path.ToString());
	checkf(IsValid(loadedActor), TEXT("BlueprintFilePath is Not Valid"));

	UBlueprint* blueprintActor = Cast<UBlueprint>(loadedActor);
	checkf(IsValid(blueprintActor), TEXT("Failed Casting to Blueprint"));

	TSubclassOf<AActor> classType = (UClass*)blueprintActor->GeneratedClass;
	checkf(IsValid(blueprintActor), TEXT("Failed Casting to AActor"));

	if (m_BlueprintActorPools.Contains(classType)) // �̹� ��������� �ִٸ�
	{
		if (m_BlueprintActorPools[classType].actors.Num() >= actorCount) // ���� ��������� ��û�����ͺ��� ���ų� ���ٸ� -> ���� �����ʿ����.
		{
			return;
		}
		else // ���� ��������� ��û�����ͺ��� ���ٸ�
		{
			actorCount -= m_BlueprintActorPools[classType].actors.Num();
		}
	}
	else
	{
		FActors actorPool;
		m_BlueprintActorPools.Add(classType, actorPool);
	}

	for (int i = 0; i < actorCount; ++i)
	{
		FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }); 
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(classType, spawnTransform);
		checkf(spawnedActor->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()), TEXT("SpawnedActor doesn't inherit interfaces."));

		IPoolableActor* castedSpawnedActor = Cast<IPoolableActor>(spawnedActor);
		castedSpawnedActor->DeActivate();

		m_BlueprintActorPools[classType].actors.Add(spawnedActor);
	}
}

TWeakObjectPtr<AActor> AActorPool::SpawnActor(TSubclassOf<AActor> classType, FVector spawnLocation)
{
	checkf(IsValid(classType), TEXT("ClassType doesn't inherit from Actor"));

	if (m_ActorPools.Contains(classType) == false) 
	{
		CreateActorPool(classType, m_DefaultSpawnCount);
		UE_LOG(LogTemp, Warning, TEXT("A new actor pool was created because no actor pool was created for the requested actor."));
	}

	TWeakObjectPtr<AActor> actor = nullptr;

	for (TWeakObjectPtr<AActor> poolableActor : m_ActorPools[classType].actors)
	{
		IPoolableActor* castedPoolableActor = Cast<IPoolableActor>(poolableActor);
		checkf(castedPoolableActor != nullptr, TEXT("Failed Casting to IPoolableActor"));

		if (castedPoolableActor->GetIsActivated()) continue;

		actor = poolableActor;
		actor->SetActorLocation(spawnLocation);
		castedPoolableActor->Activate();
		break;
	}

	if (actor == nullptr) 
	{
		CreateActorPool(classType, m_ActorPools[classType].actors.Num() * 2);
		SpawnActor(classType);
		return nullptr;
	}

	return actor;
}

TWeakObjectPtr<AActor> AActorPool::SpawnBlueprintActor(FName path, FVector spawnLocation)
{
	UObject* loadedActor = StaticLoadObject(UObject::StaticClass(), nullptr, *path.ToString());
	checkf(IsValid(loadedActor), TEXT("BlueprintFilePath is Not Valid"));

	UBlueprint* blueprintActor = Cast<UBlueprint>(loadedActor);
	checkf(IsValid(blueprintActor), TEXT("Failed Casting to Blueprint"));

	TSubclassOf<AActor> classType = (UClass*)blueprintActor->GeneratedClass;
	checkf(IsValid(classType), TEXT("Failed Casting to AActor"));

	if (m_BlueprintActorPools.Contains(classType) == false) // �̹� ����������� �ƿ� ���ٸ� (Ű����ü�� ���ٸ�)
	{
		CreateBlueprintActorPool(path, m_DefaultSpawnCount);
		UE_LOG(LogTemp, Warning, TEXT("A new actor pool was created because no actor pool was created for the requested actor."));
	}

	TWeakObjectPtr<AActor> actor = nullptr;

	for (TWeakObjectPtr<AActor> poolableActor : m_BlueprintActorPools[classType].actors)
	{
		IPoolableActor* castedPoolableActor = Cast<IPoolableActor>(poolableActor);
		checkf(castedPoolableActor != nullptr, TEXT("Failed Casting to IPoolableActor"));

		if (castedPoolableActor->GetIsActivated()) continue;

		actor = poolableActor;
		actor->SetActorLocation(spawnLocation);
		castedPoolableActor->Activate();
		break;
	}

	if (actor == nullptr) 
	{
		CreateBlueprintActorPool(path, m_BlueprintActorPools[classType].actors.Num() * 2);
		SpawnBlueprintActor(path);
		return nullptr;
	}

	return actor;
}

