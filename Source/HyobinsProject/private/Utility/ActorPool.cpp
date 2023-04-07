// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ActorPool.h"

AActorPool::AActorPool()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AActorPool::BeginPlay()
{
	Super::BeginPlay();

}

//template<class T>
void AActorPool::CreatePool(TSubclassOf<AActor> classType, FString actorName, int actorCount)
{
	checkf(IsValid(classType), TEXT("ClassType does not inherit from Actor"));

	TArray<AActor*> temp;

	for (int i = 0; i < actorCount; ++i)
	{
		temp.Add(GetWorld()->SpawnActor<AActor>(classType));
		m_Actors.Add(actorName, temp);
	}
}

