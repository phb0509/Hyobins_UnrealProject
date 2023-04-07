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

	FString temp = "MeleeMinion";
	actorPool->CreatePool(AMeleeMinion::StaticClass(), "MeleeMinion", 6);

}


void AFirstLevel::spawn()
{

}


//template<class T>
//void AFirstLevel::TestFunction(T* classType)
//{
//	TSubclassOf<ACharacterBase> temp = classType;
//
//	if (temp != nullptr)
//	{
//		float x = FMath::RandRange(0, 500);
//		float y = FMath::RandRange(0, 500);
//		const FVector location = { x,y,10 };
//		const FRotator rotation = { 0,0,0 };
//		GetWorld()->SpawnActor<AActor>(classType, location, rotation);
//
//		UE_LOG(LogTemp, Warning, TEXT("parameter is Correct"));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("parameter is not correct"));
//	}
//}
