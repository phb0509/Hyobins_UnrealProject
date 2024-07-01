// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Perception/AISenseConfig_Sight.h"


ASuperMinionAIController::ASuperMinionAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	m_TeamID = FGenericTeamId(4);
	initAssets();
}

void ASuperMinionAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UE_LOG(LogTemp, Warning, TEXT("ASuperMinionAIController :: PostInitializeComponents"));
	
}

void ASuperMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("ASuperMinionAIController :: BeginPlay"));
}

void ASuperMinionAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	m_Owner = Cast<ASuperMinion>(pawn);

	GetAIPerceptionComponent()->Activate();
	GetAIPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
	
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
	
	GetAIPerceptionComponent()->Deactivate();
	GetAIPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
}

void ASuperMinionAIController::UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus)
{
	if (m_Owner.IsValid())
	{
		ACharacterBase* const perceivedCharacter = Cast<ACharacterBase>(actor);

		if (perceivedCharacter != nullptr)
		{
			const int teamType = GetTeamAttitudeTowards(*actor);
			FString teamTypeName = "";

			switch (teamType) // ������ ��ü�� ���� ���� �̺�Ʈó��.
			{
			case 0:  // ����
			{
				teamTypeName = "Friendly::";
			}
			break;

			case 1:  // �߸�
			{
				teamTypeName = "Neutral::";
			}
			break;

			case 2:  // ��. ���� ��ϵ� ���� �÷��̾�ۿ� ����.
			{
				const ACharacterBase* const enemyOnBlackBoard = Cast<ACharacterBase>(Blackboard->GetValueAsObject(AMonster::EnemyKey));

				if (enemyOnBlackBoard == nullptr) // �þ߹��� ������ ���� ������ ��
				{
					Blackboard->SetValueAsObject(AMonster::EnemyKey, perceivedCharacter);
				}
				else // �þ߹��� ������ ���� ������ �� or ���� �ȿ��ִµ� �ٸ� ��ü�� ���� �������� ���
				{
					
				}

				teamTypeName = "Enemy::";
			}
			break;

			default:
				break;
			}

			// �α����
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

void ASuperMinionAIController::initAssets()
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

	auto perceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	perceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASuperMinionAIController::UpdatePerceptedTargetActor);
	
	UAISenseConfig_Sight* sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	
	sightConfig->SightRadius = 1000.0f;
	sightConfig->LoseSightRadius = 1100.0f;
	sightConfig->PeripheralVisionAngleDegrees = 180.0f; // �þ߰�
	sightConfig->SetMaxAge(5.0f);
	sightConfig->AutoSuccessRangeFromLastSeenLocation = 0.0f; // �����ϴ� �󵵼�? 0�̸� �ǽð� ������, ���� �������� �� üũ�Ѵ�.
															  // �� ���� 0���� ũ�ٸ�, AI�� �� �� �߰��� Ÿ���� ���� ������ ���� ���� �ִ� �� �׻� �� �� �ֽ��ϴ�.
	
	sightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	sightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true; 
	
	perceptionComponent->ConfigureSense(*sightConfig);
	perceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass()); // ��� �켱������ �������� ����.
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