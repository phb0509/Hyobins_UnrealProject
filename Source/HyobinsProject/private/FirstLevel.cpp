// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLevel.h"
#include "SubSystems/ActorPoolManager.h"
#include "ActorPool/ActorPool.h"
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

	const auto actorPoolManager = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolManager>();
	checkf(IsValid(actorPoolManager), TEXT("AFirstLevel :: ActorPoolManager isn't Valid"));

	m_ActorPool = actorPoolManager->GetActorPool();
	
	checkf(m_ActorPool->IsValidLowLevel(), TEXT("AFirstLevel :: ActorPool isn't Valid"));

	Create();
}

void AFirstLevel::Create()
{
	m_ActorPool->CreateBlueprintActorPool("Blueprint'/Game/MonsterAsset/SuperMinion/BP_SuperMinion.BP_SuperMinion'", 1);
}

void AFirstLevel::Spawn()
{
	FVector positions[4] = { {700.0f,0.0f,50.0f}, {-700.0f,0.0f,50.0f}, {0.0f,700.0f,50.0f}, {0.0f,-700.0f,50.0f} };

	for (int i = 0; i < 1; ++i)
	{
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
