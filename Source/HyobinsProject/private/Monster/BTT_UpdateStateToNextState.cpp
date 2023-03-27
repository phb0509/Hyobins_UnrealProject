// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTT_UpdateStateToNextState.h"
#include "Utility/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Monster.h"


UBTT_UpdateStateToNextState::UBTT_UpdateStateToNextState()
{
	NodeName = TEXT("UpdateStateToNextState");
}

EBTNodeResult::Type UBTT_UpdateStateToNextState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));

	owner->SetStateToNextState("test");
	//OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMonster::NextStateKey, static_cast<uint8>(ENormalMinionStates::Patrol));

	return EBTNodeResult::Succeeded;
}
