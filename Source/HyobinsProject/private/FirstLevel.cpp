// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "HPGameInstance.h"
#include "Utility/ActorPool.h"
#include "SubSystems/UIManager.h"


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
	checkf(IsValid(gameInstance), TEXT("AFirstLevel :: GameInstance isn't Valid"));

	m_ActorPool = gameInstance->GetActorPool();
	
	checkf(m_ActorPool->IsValidLowLevel(), TEXT("AFirstLevel :: ActorPool isn't Valid"));

	Create();
}

void AFirstLevel::Create()
{
	m_ActorPool->CreateBlueprintActorPool("Blueprint'/Game/MonsterAsset/Minion/BP_MeleeMinion.BP_MeleeMinion'", 4);
	m_ActorPool->CreateBlueprintActorPool("Blueprint'/Game/MonsterAsset/SuperMinion/BP_SuperMinion.BP_SuperMinion'", 4);
}

void AFirstLevel::Spawn()
{
	FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f}, {0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };

	for (int i = 0; i < 4; ++i)
	{
		m_ActorPool->SpawnBlueprintActor("Blueprint'/Game/MonsterAsset/Minion/BP_MeleeMinion.BP_MeleeMinion'", positions[i]);
		m_ActorPool->SpawnBlueprintActor("Blueprint'/Game/MonsterAsset/SuperMinion/BP_SuperMinion.BP_SuperMinion'", positions[i]);
	}
}

void AFirstLevel::ClearActorPool()
{
	m_ActorPool->ClearBlueprintActorPool();
}

void AFirstLevel::HideHPBar()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->HideWidgets("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'");
}

void AFirstLevel::ShowHPBar()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ShowWidgets("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'");
}

void AFirstLevel::ClearAllWidget()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ShowWidgets("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'");
}

void AFirstLevel::ClearHPBarWidget()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ClearAllWidgets();
}
