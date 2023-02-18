// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AIControllerBase.h"

//const FName AAIControllerBase::HomePosKey(TEXT("HomePos"));
//const FName AAIControllerBase::PatrolPosKey(TEXT("PatrolPos"));


AAIControllerBase::AAIControllerBase()
{
	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject
	(TEXT("BehaviorTree'/Game/MonsterAsset/Minion/BT_MeleeMinion.BT_MeleeMinion'"));
	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BTObject"));
	}

	checkf(IsValid(m_BehaviorTree), TEXT("m_BehaviorTree is not Valid"));*/

	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject
	//(TEXT("BlackboardData'/Game/MonsterAsset/Minion/BB_MeleeMinion.BB_MeleeMinion'"));
	//if (BBObject.Succeeded())
	//{
	//	m_BlackboardData = BBObject.Object;
	//	UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BBObject"));
	//}
	//	
	//checkf(IsValid(m_BlackboardData), TEXT("m_BlackboardData is not Valid"));

	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	checkf(IsValid(m_BehaviorTreeComponent), TEXT("m_BehaviorTreeComponent is not Valid"));
}

//void AAIControllerBase::BeginPlay()
//{
//	Super::BeginPlay();
//	RunBehaviorTree(m_BehaviorTree);
//	m_BehaviorTreeComponent->StartTree(*m_BehaviorTree);
//}

//void AAIControllerBase::OnPossess(APawn* pawn)
//{
//	Super::OnPossess(pawn);
//
//	UBlackboardComponent* BlackboardComponent = Blackboard;
//
//	BlackboardComponent->InitializeBlackboard(*m_BehaviorTree->BlackboardAsset);
//
//	UE_LOG(LogTemp, Warning, TEXT("OnPosses!!!"));
//
//	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
//	{
//		Blackboard->SetValueAsVector(HomePosKey, pawn->GetActorLocation());
//		UE_LOG(LogTemp, Warning, TEXT("if UseBlackboard.... in true"));
//	}
//}

void AAIControllerBase::LoadBehaviorTree(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject
	(*assetPath);
	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BTObject"));
	}

	checkf(IsValid(m_BehaviorTree), TEXT("m_BehaviorTree is not Valid"));
}

void AAIControllerBase::LoadBlackBoard(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject
	(*assetPath);
	if (BBObject.Succeeded())
	{
		m_BlackboardData = BBObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BBObject"));
	}

	checkf(IsValid(m_BlackboardData), TEXT("m_BlackboardData is not Valid"));
}
