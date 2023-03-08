// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "MainPlayer/MainPlayer.h"


const FName AMeleeMinionAIController::HomePosKey(TEXT("HomePos"));
const FName AMeleeMinionAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMeleeMinionAIController::PlayerKey(TEXT("Player"));
const FName AMeleeMinionAIController::InNormalAttackRangeKey(TEXT("InNormalAttackRange"));

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
	m_Owner = pawn;

	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset);

	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(HomePosKey, pawn->GetActorLocation());
	}
}

ETeamAttitude::Type AMeleeMinionAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == FGenericTeamId(4))
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile; // 적
			}
		}
	}
	return ETeamAttitude::Neutral; // 중립
}





void AMeleeMinionAIController::CheckIsTarget(AActor* actor, FAIStimulus const Stimulus)
{
	//if (auto const target = Cast<AMainPlayer>(actor)) // 탐지한 객체가 타겟(플레이어)라면
	//{
	//	//성공적으로 감지하면 블랙보드에 true값을 넣어준다.
	//	//Blackboard->SetValueAsBool(TargetKey, Stimulus.WasSuccessfullySensed());

	//	//Stimulus.

	//	Blackboard->SetValueAsObject(PlayerKey, target);
	//	//Blackboard->SetValueAsBool("IsDetect", Stimulus.WasSuccessfullySensed());
	//	//Blackboard->SetValueAsBool("IsDetect", true);
	//}
	//else
	//{
	//	Blackboard->SetValueAsObject(PlayerKey, nullptr);
	//}
	

	//if (GetTeamAttitudeTowards(actor) == )

	if (m_Owner.IsValid())
	{
		ACharacterBase* owner = Cast<ACharacterBase>(m_Owner);

		FString temp = owner->GetName() + " sensing " + Cast<ACharacterBase>(actor)->GetName();

		switch (Stimulus.Type)
		{
		case 0: // react to sight stimulus
		{
			temp += " Using SightPerception";
			break;
		}
			
		case 1: // react to hearing;
		{
			temp += " Using HearingPerception";
			break;
		}
			
		default:
			break;
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *temp);
	}
}

void AMeleeMinionAIController::initPerceptionSystem()
{
	m_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	checkf(IsValid(m_SightConfig), TEXT("SightConfig is not Valid"));

	m_SightRadius = 600.0f;
	m_LoseSightRadius = 700.0f;
	m_PeripheralVisionHalfAngle = 90.0f;
	m_AILastSeenLocation = 0.0f;
	m_AISightAge = 0.0f;

	checkf(IsValid(m_AIPerceptionComponent), TEXT("AIPerceptionComponent is not Valid"));
	SetPerceptionComponent(*m_AIPerceptionComponent);

	m_SightConfig->SightRadius = m_SightRadius;
	m_SightConfig->LoseSightRadius = m_LoseSightRadius;
	m_SightConfig->PeripheralVisionAngleDegrees = m_PeripheralVisionHalfAngle;
	m_SightConfig->SetMaxAge(m_AISightAge);
	m_SightConfig->AutoSuccessRangeFromLastSeenLocation = m_AILastSeenLocation; // 감지하는 빈도수? 0이면 실시간 감지고, 값이 높을수록 덜 체크한다.
																				// 이 값이 0보다 크다면, AI는 한 번 발견한 타깃이 여기 지정된 범위 내에 있는 한 항상 볼 수 있습니다.

	m_SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 적
	m_SightConfig->DetectionByAffiliation.bDetectNeutrals = false; // 중립
	m_SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 아군

	GetPerceptionComponent()->ConfigureSense(*m_SightConfig);
	GetPerceptionComponent()->SetDominantSense(UAISenseConfig_Sight::StaticClass()); // 어떤걸 우선순위로 센싱할지 정함.
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMeleeMinionAIController::CheckIsTarget);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMeleeMinionAIController::UpdatePerception);
	AAIController::SetGenericTeamId(FGenericTeamId(4));

}