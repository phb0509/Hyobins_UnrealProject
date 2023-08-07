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


public:
	// �߰�ȣ���� �� ���, �߰�ȣ���� 'actorCount - ���������� ������ ����' ��ŭ�� �����Ѵ�.
	void CreateActorPool(TSubclassOf<AActor> classType, int actorCount);
	void CreateBlueprintActorPool(FName path, int actorCount);

	TWeakObjectPtr<AActor> SpawnActor(TSubclassOf<AActor> classType, FVector spawnLocation = {0.0f, 0.0f, 0.0f});
	TWeakObjectPtr<AActor> SpawnBlueprintActor(FName path, FVector spawnLocation = { 0.0f, 0.0f, 0.0f });

private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActors> m_ActorPools;

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActors> m_BlueprintActorPools;

	int32 m_DefaultSpawnCount;
};


