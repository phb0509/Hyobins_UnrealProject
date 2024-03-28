// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/AIControllerBase.h"
//#include "Navigation/CrowdFollowingComponent.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_TeamID(FGenericTeamId(0)),
	m_SightRadius(300.0f),
	m_LoseSightRadius(400.0f),
	m_PeripheralVisionHalfAngle(90.0f),
	m_AISightAge(5.0f),
	m_AILastSeenLocation(900.0f)
{
	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree_Component"));
	checkf(IsValid(m_BehaviorTreeComponent.Get()), TEXT("m_BehaviorTreeComponent is not Valid"));

	m_AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception_Component");
	checkf(IsValid(m_AIPerceptionComponent.Get()), TEXT("AIPerceptionComponent is not Valid"));
	
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
	UE_LOG(LogTemp, Warning, TEXT("AIControllerBase :: OnPossess"));

	m_AIPerceptionComponent->Activate();
	m_AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogTemp, Warning, TEXT("AIControllerBase :: OnUnPossess"));

	m_AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
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

