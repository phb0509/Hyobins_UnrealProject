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

	check(IsValid(m_BehaviorTree));
	this->RunBehaviorTree(m_BehaviorTree.Get());
	
	check(IsValid(m_BehaviorTreeComponent))
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);

	check(IsValid(m_AIPerceptionComponent))
}

void AAIControllerBase::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	m_Owner = Cast<ACharacterBase>(pawn);
	check(m_Owner != nullptr);
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAIControllerBase::StartBehaviorTree()
{
	this->RunBehaviorTree(m_BehaviorTree.Get());
	
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
}

void AAIControllerBase::StopBehaviorTree()
{
	m_BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	
	this->StopMovement();
}


void AAIControllerBase::RestartBehaviorTree() const
{
	m_BehaviorTreeComponent->RestartTree();
}

