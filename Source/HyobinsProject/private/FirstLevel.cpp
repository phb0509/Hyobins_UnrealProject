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

	m_ActorPool = gameInstance->GetActorPool();
	
	checkf(m_ActorPool->IsValidLowLevel(), TEXT("AFirstLevel :: actorPool is not Valid"));

	m_ActorPool->CreatePool(AMeleeMinion::StaticClass(), 3);
	m_ActorPool->CreatePool(ASuperMinion::StaticClass(), 3);
}


void AFirstLevel::spawn()
{
	FVector meleeSpawnLocation = { 0.0f, 100.0f, 100.0f };
	m_ActorPool->SpawnActor(AMeleeMinion::StaticClass(), meleeSpawnLocation);

	FVector superSpawnLocation = { 0.0f, 0.0f, 100.0f };
	m_ActorPool->SpawnActor(ASuperMinion::StaticClass(), superSpawnLocation);
}



