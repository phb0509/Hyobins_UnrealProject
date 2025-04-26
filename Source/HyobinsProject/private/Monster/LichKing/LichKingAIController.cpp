// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/LichKingAIController.h"
#include "Monster/LichKing/LichKing.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

ALichKingAIController::ALichKingAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	m_TeamID = FGenericTeamId(4);
	initAssets();
}

void ALichKingAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ALichKingAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	m_Owner = Cast<ALichKing>(pawn);

	GetAIPerceptionComponent()->Activate();
	GetAIPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
	
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset); // 드래그로 레벨에 갖다놓으면 블랙보드컴포넌트가 null.
}

void ALichKingAIController::OnUnPossess()
{
	Super::OnUnPossess();

	GetAIPerceptionComponent()->Deactivate();
	GetAIPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
}

void ALichKingAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALichKingAIController::UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus)
{
	ACharacterBase* const perceivedCharacter = Cast<ACharacterBase>(actor);

	if (perceivedCharacter != nullptr)
	{
		const int teamType = GetTeamAttitudeTowards(*actor);
		FString teamTypeName = "";

		switch (teamType) // 인지한 객체의 팀에 따른 이벤트처리.
		{
		case 0: // 동료
			{
				teamTypeName = "Friendly::";
			}
			break;

		case 1: // 중립
			{
				teamTypeName = "Neutral::";
			}
			break;

		case 2: // 적. 현재 등록된 적은 플레이어밖에 없다.
			{
				teamTypeName = "Enemy::";

				const ACharacterBase* const enemyOnBlackBoard = Cast<ACharacterBase>(
					Blackboard->GetValueAsObject(AMonster::EnemyKey));

				if (enemyOnBlackBoard == nullptr) // 시야범위 안으로 적이 들어왔을 때
				{
					Blackboard->SetValueAsObject(AMonster::EnemyKey, perceivedCharacter);
				}
			}
			break;

		default:
			break;
		}

		// 로그출력
		FString log = "'" + m_Owner->Tags[0].ToString() + "'" + " Sensing " + "'" + teamTypeName + perceivedCharacter->
			Tags[0].ToString() + "'";

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

void ALichKingAIController::initAssets()
{
	AAIController::SetGenericTeamId(m_TeamID);
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/MonsterAsset/LichKing/BT_LichKing.BT_LichKing'"));
	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
	}
	checkf(IsValid(m_BehaviorTree.Get()), TEXT("BehaviorTree isn't Valid"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/MonsterAsset/LichKing/BB_LichKing.BB_LichKing'"));
	if (BBObject.Succeeded())
	{
		m_BlackboardData = BBObject.Object;
	}
	checkf(IsValid(m_BlackboardData.Get()), TEXT("BlackboardData isn't Valid"));

	m_AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ALichKingAIController::UpdatePerceptedTargetActor);

	int temp = 0;
}

ETeamAttitude::Type ALichKingAIController::GetTeamAttitudeTowards(const AActor& Other) const
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
