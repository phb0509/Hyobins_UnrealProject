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
	
	checkf(m_ActorPool->IsValidLowLevel(), TEXT("AFirstLevel :: ActorPool is not Valid"));


	m_ActorPool->CreateBlueprintActorPool("Blueprint'/Game/MonsterAsset/Minion/BP_MeleeMinion.BP_MeleeMinion'", 1);
	//m_ActorPool->CreateBlueprintActorPool("Blueprint'/Game/MonsterAsset/Minion/BP_MeleeMinion.BP_MeleeMinion'", 10);
	//m_ActorPool->CreateBlueprintActorPool("Blueprint'/Game/MonsterAsset/SuperMinion/BP_SuperMinion.BP_SuperMinion'", 1);

}


void AFirstLevel::spawn()
{
	FVector meleeSpawnLocation = { 0.0f, 100.0f, 100.0f };
	FVector superSpawnLocation = { 0.0f, 0.0f, 100.0f };

	m_ActorPool->SpawnBlueprintActor("Blueprint'/Game/MonsterAsset/Minion/BP_MeleeMinion.BP_MeleeMinion'", superSpawnLocation);
	//m_ActorPool->SpawnBlueprintActor("Blueprint'/Game/MonsterAsset/SuperMinion/BP_SuperMinion.BP_SuperMinion'", meleeSpawnLocation);
}



