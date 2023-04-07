// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ActorPool.h"
#include "Utility/PoolableActor.h"
#include "Kismet/GameplayStatics.h"

AActorPool::AActorPool()
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

	TArray<AActor*> temp;

	for (int i = 0; i < actorCount; ++i)
	{
		FTransform spawnTransform({ 0.0f,0.0f, 0.0f }, { 0.0f,0.0f,1000.0f }); // Rotation, Location

		AActor* actor = GetWorld()->SpawnActorDeferred<AActor>(classType, spawnTransform);
		
		if (actor->GetClass()->ImplementsInterface(UPoolableActor::StaticClass()))
		{
			auto pooldableActor = Cast<IPoolableActor>(actor);
			pooldableActor->DeActivate();

			temp.Add(actor);

			m_Actors.Add(classType, temp);
		}

		UGameplayStatics::FinishSpawningActor(actor, { {0.0f,0.0f,0.0f}, {100.0f,100.0f,30.0f} });

		checkf(IsValid(actor), TEXT("Actor does not inherit from IPoolableActor"));
	}
}

