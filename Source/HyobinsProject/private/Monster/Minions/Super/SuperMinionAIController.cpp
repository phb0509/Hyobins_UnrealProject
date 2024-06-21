// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Perception/AISenseConfig_Sight.h"


ASuperMinionAIController::ASuperMinionAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	m_TeamID = FGenericTeamId(4);
	initPerceptionSystem();
}

void ASuperMinionAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UE_LOG(LogTemp, Warning, TEXT("SuperMinionAIController::OnPossess"));
	
	m_Owner = Cast<ASuperMinion>(pawn);
	
	m_AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
	
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset);
	
	if (UseBlackboard(m_BlackboardData.Get(), BlackboardComponent))
	{
		Blackboard->SetValueAsVector(AMonster::HomePosKey, pawn->GetActorLocation());
	}
}

void ASuperMinionAIController::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogTemp, Warning, TEXT("SuperMinionAIController::OnUnPossess"));
	
	m_AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
}

void ASuperMinionAIController::UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("SuperMinionAIController::UpdatePerceptedTargetActor"));
	
	if (m_Owner.IsValid())
	{
		ACharacterBase* const perceivedCharacter = Cast<ACharacterBase>(actor);

		if (perceivedCharacter != nullptr)
		{
			const int teamType = GetTeamAttitudeTowards(*actor);
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
				const ACharacterBase* const enemyOnBlackBoard = Cast<ACharacterBase>(Blackboard->GetValueAsObject(AMonster::EnemyKey));

				if (enemyOnBlackBoard == nullptr) // 시야범위 안으로 적이 들어왔을 때
				{
					Blackboard->SetValueAsObject(AMonster::EnemyKey, perceivedCharacter);
				}
				else // 시야범위 밖으로 적이 나갔을 때 or 범위 안에있는데 다른 물체에 의해 가려졌을 경우
				{
					
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

			//UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
		}
	}
}

void ASuperMinionAIController::initPerceptionSystem()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/MonsterAsset/SuperMinion/BT_SuperMinion.BT_SuperMinion'"));
	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
	}
	checkf(IsValid(m_BehaviorTree.Get()), TEXT("BehaviorTree isn't Valid"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/MonsterAsset/SuperMinion/BB_SuperMinion.BB_SuperMinion'"));
	if (BBObject.Succeeded())
	{
		m_BlackboardData = BBObject.Object;
	}
	checkf(IsValid(m_BlackboardData.Get()), TEXT("BlackboardData isn't Valid"));

	AAIController::SetGenericTeamId(m_TeamID);

	//SetPerceptionComponent(*m_AIPerceptionComponent);
	
	UAISenseConfig_Sight* sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	
	sightConfig->SightRadius = 1000.0f;
	sightConfig->LoseSightRadius = 1100.0f;
	sightConfig->PeripheralVisionAngleDegrees = 180.0f; // 시야각
	sightConfig->SetMaxAge(5.0f);
	sightConfig->AutoSuccessRangeFromLastSeenLocation = 0.0f; // 감지하는 빈도수? 0이면 실시간 감지고, 값이 높을수록 덜 체크한다.
																				// 이 값이 0보다 크다면, AI는 한 번 발견한 타깃이 여기 지정된 범위 내에 있는 한 항상 볼 수 있습니다.
	
	sightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	sightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true; 
	
	m_AIPerceptionComponent->ConfigureSense(*sightConfig);
	m_AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass()); // 어떤걸 우선순위로 센싱할지 정함.
	m_AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASuperMinionAIController::UpdatePerceptedTargetActor);
	
}

ETeamAttitude::Type ASuperMinionAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* otherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* teamAgent = Cast<IGenericTeamAgentInterface>(otherPawn->GetController()))
		{
			if (teamAgent->GetGenericTeamId() == FGenericTeamId(4))
			{
				return ETeamAttitude::Friendly; 
			}
			else
			{
				return ETeamAttitude::Hostile; 
			}
		}
	}
	return ETeamAttitude::Neutral; 
}