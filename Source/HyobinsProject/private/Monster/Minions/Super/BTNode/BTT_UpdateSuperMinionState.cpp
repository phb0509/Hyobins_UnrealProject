// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTNode/BTT_UpdateSuperMinionState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "CharacterBase/AIControllerBase.h"
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
	check(owner != nullptr);
	
	const ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));
	check(enemyOnBlackBoard != nullptr);
	
	const float distanceToEnemy = owner->GetDistanceTo(enemyOnBlackBoard);
	
	if (distanceToEnemy > m_NormalAttackRange)
	{
		owner->SetFSMState(ESuperMinionFSMStates::Chase); 
	}
	else if (distanceToEnemy <= m_NormalAttackRange)
	{
		owner->SetFSMState(ESuperMinionFSMStates::NormalAttack);
	}

	return EBTNodeResult::Succeeded;
}