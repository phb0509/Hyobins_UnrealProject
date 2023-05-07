// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "Monster/Minions/Melee/MeleeMinion.h"

AMeleeMinionAIController::AMeleeMinionAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	m_TeamID = FGenericTeamId(4);

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/MonsterAsset/Minion/BT_MeleeMinion.BT_MeleeMinion'"));
	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
	}
	checkf(IsValid(m_BehaviorTree), TEXT("BehaviorTree is not Valid"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/MonsterAsset/Minion/BB_MeleeMinion.BB_MeleeMinion'"));
	if (BBObject.Succeeded())
	{
		m_BlackboardData = BBObject.Object;
	}
	checkf(IsValid(m_BlackboardData), TEXT("BlackboardData is not Valid"));

	initPerceptionSystem();
}

void AMeleeMinionAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	UE_LOG(LogTemp, Warning, TEXT("Call MeleeMinionAIController::OnPossess"));

	UBlackboardComponent* BlackboardComponent = Blackboard;
	
	m_Owner = Cast<AMeleeMinion>(pawn);

	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset);
	
	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(AMonster::HomePosKey, pawn->GetActorLocation());
	}
}

void AMeleeMinionAIController::UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Call MeleeMinionAIController::UpdatePerceptedActor"));

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

			case 2:  // 적
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

void AMeleeMinionAIController::initPerceptionSystem()
{
	m_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	checkf(IsValid(m_SightConfig), TEXT("SightConfig is not Valid"));

	m_SightRadius = 800.0f;
	m_LoseSightRadius = 1500.0f;
	m_PeripheralVisionHalfAngle = 180.0f;
	m_AILastSeenLocation = 0.0f;
	m_AISightAge = 5.0f; // 비활성화 후, 재스폰했을 때 다시 인지시키려면 0값이면 안된다.

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

	m_AIPerceptionComponent->ConfigureSense(*m_SightConfig);
	m_AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass()); // 어떤걸 우선순위로 센싱할지 정함.
	m_AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::UpdatePerceptedTargetActor);
	
	//m_AIPerceptionComponent->Deactivate();
	
	AAIController::SetGenericTeamId(m_TeamID);


	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMeleeMinionAIController::UpdatePerception);
}

ETeamAttitude::Type AMeleeMinionAIController::GetTeamAttitudeTowards(const AActor& Other) const
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