// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/BTT_MeleeMinionAttack.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Minions/Melee/MeleeMinion.h"

UBTT_MeleeMinionAttack::UBTT_MeleeMinionAttack()
{
	NodeName = TEXT("MeleeMinionAttack");
}

EBTNodeResult::Type UBTT_MeleeMinionAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMeleeMinion* owner = Cast<AMeleeMinion>(OwnerComp.GetAIOwner()->GetPawn());
	checkf(IsValid(owner), TEXT("Owner is not Valid"));
	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));
	owner->NormalAttack();

	return EBTNodeResult::Succeeded;
}