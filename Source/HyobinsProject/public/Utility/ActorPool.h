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
	void CreatePool(TSubclassOf<class AActor> classType, int actorCount);



private:
	TMap<TSubclassOf<class AActor>, TArray<class AActor*>> m_Actors;
};


