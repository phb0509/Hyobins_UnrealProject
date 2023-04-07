// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "HPGameInstance.h"
#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Utility/ActorPool.h"


AFirstLevel::AFirstLevel() :
	m_SpawnTimerTime_MeleeMinion(2.0f)
{
	
}

void AFirstLevel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFirstLevel::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AFirstLevel :: BeginPlay"));

	UHPGameInstance* gameInstance = Cast<UHPGameInstance>(GetGameInstance());
	checkf(IsValid(gameInstance), TEXT("AFirstLevel :: GameInstance is not Valid"));

	AActorPool* actorPool = gameInstance->GetActorPool();
	checkf(IsValid(actorPool), TEXT("AFirstLevel :: actorPool is not Valid"));

	actorPool->CreatePool(AMeleeMinion::StaticClass(), 6);
}


void AFirstLevel::spawn()
{

}

