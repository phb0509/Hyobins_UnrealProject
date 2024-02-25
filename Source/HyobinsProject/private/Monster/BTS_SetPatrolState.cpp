// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTS_SetPatrolState.h"
#include "Monster/Monster.h"
#include "Utility/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTS_SetPatrolState::UBTS_SetPatrolState()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("SetPatrolState");
}

void UBTS_SetPatrolState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AMonster* const owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	checkf(IsValid(owner), TEXT("Monster isn't Valid"));
	owner->SetCommonState(EMonsterCommonStates::Patrol);

}
