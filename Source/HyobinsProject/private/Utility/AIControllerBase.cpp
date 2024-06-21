// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/AIControllerBase.h"


AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_TeamID(FGenericTeamId(0)),
	m_SightRadius(300.0f),
	m_LoseSightRadius(400.0f),
	m_PeripheralVisionHalfAngle(90.0f),
	m_AISightAge(5.0f),
	m_AILastSeenLocation(900.0f)
{
	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	m_AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*m_AIPerceptionComponent);
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AIControllerBase :: BeginPlay"));

	RunBehaviorTree(m_BehaviorTree.Get());
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
}

void AAIControllerBase::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	
	m_AIPerceptionComponent->Activate();
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
	
	m_AIPerceptionComponent->Deactivate();
}

void AAIControllerBase::StopBehaviorTree() const
{
	m_BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void AAIControllerBase::StartBehaviorTree()
{
	RunBehaviorTree(m_BehaviorTree.Get());
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
}

