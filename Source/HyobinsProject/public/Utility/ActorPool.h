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
	// �߰�ȣ���� �� ���, �߰�ȣ���� 'actorCount - ���������� ������ ����' ��ŭ�� �����Ѵ�.
	void CreateActorPool(TSubclassOf<AActor> classType, int actorCount);
	void CreateBlueprintActorPool(FName path, int actorCount);

	AActor* SpawnActor(TSubclassOf<AActor> classType, FVector spawnLocation = {0.0f, 0.0f, 0.0f});
	AActor* SpawnBlueprintActor(FName path, FVector spawnLocation = { 0.0f, 0.0f, 0.0f });

private:
	TMap<TSubclassOf<AActor>, TArray<AActor*>> m_ActorPools;
	TMap<TSubclassOf<AActor>, TArray<AActor*>> m_BlueprintActorPools;

	int32 m_DefaultSpawnCount;
};


