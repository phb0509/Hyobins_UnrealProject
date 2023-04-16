// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ActorPool.h"
#include "Utility/PoolableActor.h"
#include "Kismet/GameplayStatics.h"

AActorPool::AActorPool() :
	m_DefaultSpawnCount(10)
{
	PrimaryActorTick.bCanEverTick = false;

}

void AActorPool::BeginPlay()
{
	Super::BeginPlay();
}

void AActorPool::CreatePool(TSubclassOf<AActor> classType, int actorCount)
{
	checkf(IsValid(classType), TEXT("ClassType does not inherit from Actor"));

	if (classType->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()) == false)
	{
		checkf(true, TEXT("ClassType does not inherit from IPoolableActor"));
	}

	if (m_ActorPools.Contains(classType)) // 이미 만들어진게 있다면
	{
		if (m_ActorPools[classType].Num() >= actorCount) // 현재 만들어진게 요청받은것보다 많거나 같다면 -> 새로 만들필요없다.
		{
			return;
		}
		else // 현재 만들어진게 요청받은것보다 적다면
		{
			actorCount -= m_ActorPools[classType].Num();
		}
	}
	else
	{
		TArray<AActor*> actorPool;
		m_ActorPools.Add(classType, actorPool);
	}

	for (int i = 0; i < actorCount; ++i)
	{
		//FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, i * 100.0f, 100.0f }); // Rotation, Location . 일단 임시값. DeActivate가 제대로 작성되면 0,0,0으로 해도상관없다.
		FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }); // Rotation, Location . 일단 임시값. DeActivate가 제대로 작성되면 0,0,0으로 해도상관없다.
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(classType, spawnTransform);
		IPoolableActor* castedSpawnedActor = Cast<IPoolableActor>(spawnedActor);
		castedSpawnedActor->DeActivate();

		m_ActorPools[classType].Add(spawnedActor);
	}
}

AActor* AActorPool::SpawnActor(TSubclassOf<AActor> classType, FVector spawnLocation)
{
	checkf(IsValid(classType), TEXT("ClassType does not inherit from Actor"));

	if (classType->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()) == false)
	{
		checkf(true, TEXT("ClassType does not inherit from IPoolableActor"));
	}

	if (m_ActorPools.Contains(classType) == false) // 이미 만들어진게 없다면
	{
		CreatePool(classType, m_DefaultSpawnCount);
		UE_LOG(LogTemp, Warning, TEXT("A new actor pool was created because no actor pool was created for the requested actor."));
	}

	AActor* actor = nullptr;

	for (AActor* poolableActor : m_ActorPools[classType])
	{
		IPoolableActor* castedPoolableActor = Cast<IPoolableActor>(poolableActor);

		if (castedPoolableActor->GetIsActivated()) continue;

		actor = poolableActor;
		actor->SetActorLocation(spawnLocation);
		castedPoolableActor->Activate();
		break;
	}

	if (actor == nullptr) // 가용가능한 액터가 없다면
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no spawnable Actors. Please increase the size of the ActorPool"));
		return nullptr;
	}
	
	return actor;
}

