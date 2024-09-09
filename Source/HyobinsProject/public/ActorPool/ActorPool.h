// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"


USTRUCT()
struct FActors
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly)
	TArray<TWeakObjectPtr<AActor>> actors;
};

UCLASS()
class HYOBINSPROJECT_API AActorPool : public AActor
{
	GENERATED_BODY()

public:
	AActorPool();
	
	void CreateActorPool(TSubclassOf<AActor> classType, int actorCount);
	void CreateBlueprintActorPool(const TSubclassOf<AActor> classType, int actorCount);

	TWeakObjectPtr<AActor> SpawnActor(TSubclassOf<AActor> classType, const FVector& spawnLocation = {0.0f, 0.0f, 0.0f});
	TWeakObjectPtr<AActor> SpawnBlueprintActor(const TSubclassOf<AActor> classType, const FVector& spawnLocation = { 0.0f, 0.0f, 0.0f });

	void ClearActorPool();
	void ClearBlueprintActorPool();

	void CreateUsingClass(TSubclassOf<AActor> classType);
	
private:
	UPROPERTY(VisibleInstanceOnly)
	TMap<TSubclassOf<AActor>, FActors> m_ActorPool;

	UPROPERTY(VisibleInstanceOnly)
	TMap<TSubclassOf<AActor>, FActors> m_BlueprintActorPool;

	int32 m_DefaultSpawnCount;
};


