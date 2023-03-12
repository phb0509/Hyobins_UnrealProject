// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/BTS_UpdateMeleeMinionState.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Minions/Melee/MeleeMinion.h"


UBTS_UpdateMeleeMinionState::UBTS_UpdateMeleeMinionState()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("UpdateMeleeMinionState");
}

void UBTS_UpdateMeleeMinionState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AMeleeMinion* meleeMinion = Cast<AMeleeMinion>(OwnerComp.GetAIOwner()->GetPawn());
	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Enemy"));
	
	float distanceToEnemy = meleeMinion->GetDistanceTo(enemyOnBlackBoard);

	if (distanceToEnemy > meleeMinion->GetNormalAttackRange()) // ���ݹ��� ���̸�
	{
		meleeMinion->SetState(ENormalMinionStates::Chase); // ������ �� �ִ� �Ÿ� �� ������ ����.
	}
	else
	{
		meleeMinion->SetState(ENormalMinionStates::Attack);
	}

	
}
