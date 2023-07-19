// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_UpdateSuperMinionState.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Minions/Super/SuperMinion.h"

UBTT_UpdateSuperMinionState::UBTT_UpdateSuperMinionState()
{
	NodeName = TEXT("UpdateSuperMinionState");
}

EBTNodeResult::Type UBTT_UpdateSuperMinionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASuperMinion* owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
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