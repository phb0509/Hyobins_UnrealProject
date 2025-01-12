// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_UpdateSuperMinionState.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Utility/AIControllerBase.h"
#include "Utility/EnumTypes.h"


UBTT_UpdateSuperMinionState::UBTT_UpdateSuperMinionState():
m_NormalAttackRange(400.0f)
{
	NodeName = TEXT("UpdateSuperMinionState");
}

EBTNodeResult::Type UBTT_UpdateSuperMinionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASuperMinion* owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
	const ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));
	const float distanceToEnemy = owner->GetDistanceTo(enemyOnBlackBoard);
	
	if (distanceToEnemy > m_NormalAttackRange) // 공격범위 밖이면
	{
		owner->SetFSMState(ESuperMinionFSMStates::Chase); // 공격할 수 있는 거리 될 때까지 추적.
	}
	else if (distanceToEnemy <= m_NormalAttackRange)
	{
		owner->SetFSMState(ESuperMinionFSMStates::NormalAttack);
	}

	return EBTNodeResult::Succeeded;
}