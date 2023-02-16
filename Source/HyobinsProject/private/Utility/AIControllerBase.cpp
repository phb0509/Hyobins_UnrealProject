// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AAIControllerBase::HomePosKey(TEXT("HomePos"));
const FName AAIControllerBase::PatrolPosKey(TEXT("PatrolPos"));


AAIControllerBase::AAIControllerBase()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject
	(TEXT("BehaviorTree'/Game/MonsterAsset/Minion/BT_MeleeMinion.BT_MeleeMinion'"));
	if (BTObject.Succeeded())
		m_BehaviorTree = BTObject.Object;


	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject
	(TEXT("BlackboardData'/Game/MonsterAsset/Minion/BB_MeleeMinion.BB_MeleeMinion'"));
	if (BBObject.Succeeded())
		m_BlackboardData = BBObject.Object;
}

void AAIControllerBase::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	UBlackboardComponent* BlackboardComponent = Blackboard;

	if (UseBlackboard(m_BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(HomePosKey, pawn->GetActorLocation());
	}
}
