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
	UPROPERTY()
		TArray<TWeakObjectPtr<AActor>> actors;
};

UCLASS()
class HYOBINSPROJECT_API AActorPool : public AActor
{
	GENERATED_BODY()

public:
	AActorPool();

	// 추가호출을 할 경우, 추가호출의 'actorCount - 기존생성된 액터의 개수' 만큼만 생성한다.
	void CreateActorPool(TSubclassOf<AActor> classType, int actorCount);
	void CreateBlueprintActorPool(FName path, int actorCount);

	TWeakObjectPtr<AActor> SpawnActor(TSubclassOf<AActor> classType, FVector spawnLocation = {0.0f, 0.0f, 0.0f});
	TWeakObjectPtr<AActor> SpawnBlueprintActor(FName path, FVector spawnLocation = { 0.0f, 0.0f, 0.0f });

	void ClearActorPool();
	void ClearBlueprintActorPool();

private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActors> m_ActorPool;

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActors> m_BlueprintActorPool;

	int32 m_DefaultSpawnCount;
};


