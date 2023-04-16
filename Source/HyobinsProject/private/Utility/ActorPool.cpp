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

	if (m_ActorPools.Contains(classType)) // �̹� ��������� �ִٸ�
	{
		if (m_ActorPools[classType].Num() >= actorCount) // ���� ��������� ��û�����ͺ��� ���ų� ���ٸ� -> ���� �����ʿ����.
		{
			return;
		}
		else // ���� ��������� ��û�����ͺ��� ���ٸ�
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
		//FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, i * 100.0f, 100.0f }); // Rotation, Location . �ϴ� �ӽð�. DeActivate�� ����� �ۼ��Ǹ� 0,0,0���� �ص��������.
		FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }); // Rotation, Location . �ϴ� �ӽð�. DeActivate�� ����� �ۼ��Ǹ� 0,0,0���� �ص��������.
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

	if (m_ActorPools.Contains(classType) == false) // �̹� ��������� ���ٸ�
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

	if (actor == nullptr) // ���밡���� ���Ͱ� ���ٸ�
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no spawnable Actors. Please increase the size of the ActorPool"));
		return nullptr;
	}
	
	return actor;
}

