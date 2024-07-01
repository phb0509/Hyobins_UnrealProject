// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_UpdateSuperMinionState.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Utility/AIControllerBase.h"
#include "Utility/EnumTypes.h"


UBTT_UpdateSuperMinionState::UBTT_UpdateSuperMinionState()
{
	NodeName = TEXT("UpdateSuperMinionState");
}

EBTNodeResult::Type UBTT_UpdateSuperMinionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASuperMinion* const owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
	const ACharacterBase* const enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));
	const float distanceToEnemy = owner->GetDistanceTo(enemyOnBlackBoard);

	float normalAttackRange = 500.0f;
	if (distanceToEnemy > normalAttackRange) // ���ݹ��� ���̸�
	{
		owner->SetState(ENormalMinionStates::Chase); // ������ �� �ִ� �Ÿ� �� ������ ����.
	}
	else if (distanceToEnemy <= normalAttackRange)
	{
		owner->SetState(ENormalMinionStates::NormalAttack);
	}

	return EBTNodeResult::Succeeded;
}