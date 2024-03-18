// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_UpdateSuperMinionState.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Utility/EnumTypes.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_UpdateSuperMinionState::UBTT_UpdateSuperMinionState()
{
	NodeName = TEXT("UpdateSuperMinionState");
}

EBTNodeResult::Type UBTT_UpdateSuperMinionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASuperMinion* const owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
	checkf(IsValid(owner), TEXT("Owner isn't Valid"));

	const ACharacterBase* const enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));
	checkf(IsValid(enemyOnBlackBoard), TEXT("Failed to Cast to enemyOnBlackBoard"));

	const float distanceToEnemy = owner->GetDistanceTo(enemyOnBlackBoard);

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