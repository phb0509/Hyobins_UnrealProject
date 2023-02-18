// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAIController.h"

const FName AMeleeMinionAIController::HomePosKey(TEXT("HomePos"));
const FName AMeleeMinionAIController::PatrolPosKey(TEXT("PatrolPos"));

AMeleeMinionAIController::AMeleeMinionAIController()
{
	LoadBehaviorTree("BehaviorTree'/Game/MonsterAsset/Minion/BT_MeleeMinion.BT_MeleeMinion'");
	LoadBlackBoard("BlackboardData'/Game/MonsterAsset/Minion/BB_MeleeMinion.BB_MeleeMinion'");
}

void AMeleeMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(m_BehaviorTree);
	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
}

void AMeleeMinionAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	UBlackboardComponent* BlackboardComponent = Blackboard;

	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset);

	UE_LOG(LogTemp, Warning, TEXT("OnPosses!!!"));

	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(HomePosKey, pawn->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("if UseBlackboard.... in true"));
	}
}
