// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/BTT_UpdateMeleeMinionState.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Minions/Melee/MeleeMinion.h"

UBTT_UpdateMeleeMinionState::UBTT_UpdateMeleeMinionState()
{
	NodeName = TEXT("UpdateMeleeMinionState");
}

EBTNodeResult::Type UBTT_UpdateMeleeMinionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMeleeMinion* owner = Cast<AMeleeMinion>(OwnerComp.GetAIOwner()->GetPawn());
	checkf(IsValid(owner), TEXT("Owner is not Valid"));

	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));
	checkf(IsValid(enemyOnBlackBoard), TEXT("Cast Failed"));

	float distanceToEnemy = owner->GetDistanceTo(enemyOnBlackBoard);

	if (distanceToEnemy > owner->GetNormalAttackRange()) // ���ݹ��� ���̸�
	{
		owner->SetState(ENormalMinionStates::Chase); // ������ �� �ִ� �Ÿ� �� ������ ����.
	}
	else if (distanceToEnemy <= owner->GetNormalAttackRange())
	{
		owner->SetState(ENormalMinionStates::NormalAttack);
	}

	return EBTNodeResult::Succeeded;
}
