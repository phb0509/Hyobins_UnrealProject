// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAIController.h"

const FName AMeleeMinionAIController::HomePosKey(TEXT("HomePos"));
const FName AMeleeMinionAIController::PatrolPosKey(TEXT("PatrolPos"));

AMeleeMinionAIController::AMeleeMinionAIController()
{
	LoadBehaviorTree("BehaviorTree'/Game/MonsterAsset/Minion/BT_MeleeMinion.BT_MeleeMinion'");
	LoadBlackBoard("BlackboardData'/Game/MonsterAsset/Minion/BB_MeleeMinion.BB_MeleeMinion'");

	initPerceptionSystem();
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

	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(HomePosKey, pawn->GetActorLocation());
	}
}

void AMeleeMinionAIController::initPerceptionSystem()
{
	m_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	
	m_SightConfig->SightRadius = m_SightRadius;
	m_SightConfig->LoseSightRadius = m_LoseSightRadius;
	m_SightConfig->PeripheralVisionAngleDegrees = m_PeripheralVisionHalfAngle;

	m_SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	m_SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*m_SightConfig->GetSenseImplementation());
	//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMeleeEnemyAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*m_SightConfig);
}