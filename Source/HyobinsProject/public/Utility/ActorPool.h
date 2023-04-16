// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"

UCLASS()
class HYOBINSPROJECT_API AActorPool : public AActor
{
	GENERATED_BODY()

public:
	AActorPool();

protected:
	virtual void BeginPlay() override;

public:
	void CreatePool(TSubclassOf<AActor> classType, int actorCount);
	AActor* SpawnActor(TSubclassOf<AActor> classType, FVector spawnLocation = {0.0f,0.0f,0.0f});

private:
	TMap<TSubclassOf<AActor>, TArray<AActor*>> m_ActorPools;
	int32 m_DefaultSpawnCount;
};


