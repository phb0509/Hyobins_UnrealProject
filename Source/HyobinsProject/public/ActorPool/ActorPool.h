// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"


class IPoolableActor;

USTRUCT()
struct FActors
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly)
	TArray<AActor*> actors;
};

UCLASS()
class HYOBINSPROJECT_API AActorPool : public AActor
{
	GENERATED_BODY()

public:
	AActorPool();
	
	void CreateActorPool(TSubclassOf<AActor> classType, int requestedCapacity);

	template<typename T>
	T* SpawnActor(const TSubclassOf<AActor> classType, const FVector& spawnLocation)
	{
		return Cast<T>(spawnActor(classType, spawnLocation));
	}
	
	void ClearActorPool();


private:
	AActor* spawnActor(const TSubclassOf<AActor> classType, const FVector& spawnLocation = { 0.0f, 0.0f, 0.0f });

private:
	UPROPERTY(VisibleInstanceOnly)
	TMap<TSubclassOf<AActor>, FActors> m_ActorPool;

	int32 m_DefaultSpawnCount;
};


