// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/AIControllerBase.h"
#include "Utility/CharacterBase.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_TeamID(FGenericTeamId(0))
{
	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
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

	m_OwnerBase = Cast<ACharacterBase>(pawn);
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

