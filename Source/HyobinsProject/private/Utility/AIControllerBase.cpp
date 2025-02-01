// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/AIControllerBase.h"
#include "Utility/CharacterBase.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_TeamID(FGenericTeamId(0))
{
	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	m_AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	RunBehaviorTree(m_BehaviorTree.Get()); 
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
}

void AAIControllerBase::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	m_Owner = Cast<ACharacterBase>(pawn);
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// int32 isRunning = m_BehaviorTreeComponent->IsRunning();
	// int32 isPause = m_BehaviorTreeComponent->IsPaused();
	//
	// FString log = m_Owner->Tags[0].ToString() + " :: Is Running? :: " + FString::FromInt(isRunning);
	// FString log1 = m_Owner->Tags[0].ToString() + " :: Is Paused? :: " + FString::FromInt(isPause);
	//
	//
	// GEngine->AddOnScreenDebugMessage(405, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
	// GEngine->AddOnScreenDebugMessage(406, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log1));
}

void AAIControllerBase::StopBehaviorTree()
{
	m_BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	StopMovement();
}

void AAIControllerBase::StartBehaviorTree()
{
	RunBehaviorTree(m_BehaviorTree.Get());
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
}

