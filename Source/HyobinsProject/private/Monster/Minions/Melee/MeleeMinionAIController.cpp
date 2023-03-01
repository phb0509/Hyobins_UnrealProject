// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "MainPlayer/MainPlayer.h"


const FName AMeleeMinionAIController::HomePosKey(TEXT("HomePos"));
const FName AMeleeMinionAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMeleeMinionAIController::PlayerKey(TEXT("Target"));

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
	checkf(IsValid(m_SightConfig), TEXT("SightConfig is not Valid"));
	
	m_SightRadius = 600.0f;
	m_LoseSightRadius = 700.0f;
	m_PeripheralVisionHalfAngle = 90.0f;
	m_AILastSeenLocation = 600.0f;
	m_AISightAge = 0.0f;

	checkf(IsValid(m_AIPerceptionComponent), TEXT("AIPerceptionComponent is not Valid"));
	SetPerceptionComponent(*m_AIPerceptionComponent);

	
	m_SightConfig->SightRadius = m_SightRadius;
	m_SightConfig->LoseSightRadius = m_LoseSightRadius;
	m_SightConfig->PeripheralVisionAngleDegrees = m_PeripheralVisionHalfAngle;
	//m_SightConfig->SetMaxAge(m_AISightAge);
	//m_SightConfig->AutoSuccessRangeFromLastSeenLocation = m_AILastSeenLocation;

	m_SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	m_SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//m_SightConfig->AutoSuccessRangeFromLastSeenLocation;

	GetPerceptionComponent()->ConfigureSense(*m_SightConfig);
	//GetPerceptionComponent()->SetDominantSense(*m_SightConfig->GetSenseImplementation()); // ��� �켱������ �������� ����.
	//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMeleeMinionAIController::CheckIsTarget);
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMeleeMinionAIController::UpdatePerception);
	
}

void AMeleeMinionAIController::UpdatePerception(const TArray<AActor*>& Actors)
{
	checkf(IsValid(Blackboard), TEXT("Blackboard is not Valid"));

	int size = Actors.Num();

	for (int i = 0; i < size; ++i)
	{
		AMainPlayer* player = Cast<AMainPlayer>(Actors[i]);

		if (player != nullptr) // player ���������Ѵٸ�
		{
			player = Cast<AMainPlayer>(Blackboard->GetValueAsObject(PlayerKey)); // '������'�� ��ϵ��ִ� �� üũ.

			if (player == nullptr) // null�̴�. ��, �����ȿ� �÷��̾ ���� ���¿��� �÷��̾ �����ƴ�? -> �÷��̾ ���������� ���Դٴ� ��.
			{
				Blackboard->SetValueAsObject(PlayerKey, Actors[i]);
			}
			else // null�� �ƴϴ�. ��, �����ȿ� �÷��̾ �ִ� ���¿��� �̺�Ʈ�� �߻��ߴ�? -> �÷��̾ ���� ������ ����
			{
				Blackboard->SetValueAsObject(PlayerKey, nullptr);
			}
		}
	}
}


//void AMeleeMinionAIController::CheckIsTarget(AActor* actor, FAIStimulus const Stimulus)
//{
//	GEngine->AddOnScreenDebugMessage(14, 1.f, FColor::Green, FString::Printf(TEXT("Call the CheckIsTarget Function!!")));
//	if (auto const target = Cast<AMainPlayer>(actor)) // Ž���� ��ü�� Ÿ��(�÷��̾�)���
//	{
//		//���������� �����ϸ� �����忡 true���� �־��ش�.
//		//Blackboard->SetValueAsBool(TargetKey, Stimulus.WasSuccessfullySensed());
//
//		//Stimulus.
//
//		Blackboard->SetValueAsObject(PlayerKey, target);
//		//Blackboard->SetValueAsBool("IsDetect", Stimulus.WasSuccessfullySensed());
//		//Blackboard->SetValueAsBool("IsDetect", true);
//	}
//	else
//	{
//		Blackboard->SetValueAsObject(PlayerKey, nullptr);
//	}
//}