// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Monster/Minions/Super/SuperMinion.h"

ASuperMinionAIController::ASuperMinionAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	m_TeamID = FGenericTeamId(4);


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/MonsterAsset/SuperMinion/BT_SuperMinion.BT_SuperMinion'"));

	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BTObject"));
	}

	checkf(IsValid(m_BehaviorTree), TEXT("m_BehaviorTree is not Valid"));


	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/MonsterAsset/SuperMinion/BB_SuperMinion.BB_SuperMinion'"));
	if (BBObject.Succeeded())
	{
		m_BlackboardData = BBObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BBObject"));
	}

	checkf(IsValid(m_BlackboardData), TEXT("m_BlackboardData is not Valid"));

	initPerceptionSystem();

}

void ASuperMinionAIController::BeginPlay()
{
	Super::BeginPlay();



}

void ASuperMinionAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UE_LOG(LogTemp, Warning, TEXT("SuperMinionAIController::OnPossess"));
	UBlackboardComponent* BlackboardComponent = Blackboard;

	m_Owner = Cast<ASuperMinion>(pawn);

	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset);

	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(AMonster::HomePosKey, pawn->GetActorLocation());
	}
}

void ASuperMinionAIController::CheckIsTarget(AActor* actor, FAIStimulus const Stimulus)
{
	if (m_Owner.IsValid())
	{
		ACharacterBase* perceivedCharacter = Cast<ACharacterBase>(actor);

		if (perceivedCharacter != nullptr)
		{
			int teamType = GetTeamAttitudeTowards(*actor);
			FString teamTypeName = "";

			switch (teamType) // 인지한 객체의 팀에 따른 이벤트처리.
			{
			case 0:  // 동료
			{
				teamTypeName = "Friendly::";
			}
			break;

			case 1:  // 중립
			{
				teamTypeName = "Neutral::";
			}
			break;

			case 2:  // 적. 현재 등록된 적은 플레이어밖에 없다.
			{
				ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(Blackboard->GetValueAsObject(AMonster::EnemyKey));

				if (enemyOnBlackBoard == nullptr) // 시야범위 안으로 적이 들어왔을 때
				{
					Blackboard->SetValueAsObject(AMonster::EnemyKey, perceivedCharacter);
				}
				else // 시야범위 밖으로 적이 나갔을 때 or 범위 안에있는데 다른 물체에 의해 가려졌을 경우
				{
					if (m_Owner->GetState() == ENormalMinionStates::Chase || m_Owner->GetState() == ENormalMinionStates::NormalAttack)
					{
						// 타겟유지.
					}
					else
					{
						m_Owner->SetState(ENormalMinionStates::Patrol);
						Blackboard->SetValueAsObject(AMonster::EnemyKey, nullptr);
					}
				}

				teamTypeName = "Enemy::";
			}
			break;

			default:
				break;
			}

			// 로그출력
			FString log = "'" + m_Owner->Tags[0].ToString() + "'" + " Sensing " + "'" + teamTypeName + perceivedCharacter->Tags[0].ToString() + "'";

			switch (Stimulus.Type)
			{
			case 0: // react to sight;
			{
				log += " Using 'SightPerception'";
				break;
			}

			case 1: // react to hearing;
			{
				log += " Using 'HearingPerception'";
				break;
			}

			default:
				break;
			}

			UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
		}
	}
}

void ASuperMinionAIController::initPerceptionSystem()
{
	m_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	checkf(IsValid(m_SightConfig), TEXT("SightConfig is not Valid"));

	m_SightRadius = 1000.0f;
	m_LoseSightRadius = 1100.0f;
	m_PeripheralVisionHalfAngle = 180.0f;
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ASuperMinionAIController::CheckIsTarget);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMeleeMinionAIController::UpdatePerception);
	AAIController::SetGenericTeamId(m_TeamID);
}

ETeamAttitude::Type ASuperMinionAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == FGenericTeamId(4))
			{
				return ETeamAttitude::Friendly; // 동료
			}
			else
			{
				return ETeamAttitude::Hostile; // 적
			}
		}
	}
	return ETeamAttitude::Neutral; // 중립
}