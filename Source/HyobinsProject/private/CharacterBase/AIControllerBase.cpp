// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase/AIControllerBase.h"
#include "CharacterBase/CharacterBase.h"

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

void AAIControllerBase::RestartBehaviorTree()
{
	m_BehaviorTreeComponent->RestartTree();
}

