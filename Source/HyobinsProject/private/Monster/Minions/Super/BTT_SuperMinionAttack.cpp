// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_SuperMinionAttack.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_SuperMinionAttack::UBTT_SuperMinionAttack()
{
	NodeName = TEXT("SuperMinionAttack");
}

EBTNodeResult::Type UBTT_SuperMinionAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASuperMinion* owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
	checkf(IsValid(owner), TEXT("Owner is not Valid"));
	owner->NormalAttack();

	return EBTNodeResult::Succeeded;
}