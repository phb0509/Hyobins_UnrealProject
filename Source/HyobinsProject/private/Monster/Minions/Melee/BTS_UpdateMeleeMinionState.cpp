// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/BTS_UpdateMeleeMinionState.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Minions/Melee/MeleeMinion.h"


UBTS_UpdateMeleeMinionState::UBTS_UpdateMeleeMinionState()
{
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
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(ENormalMinionStates::Chase));
	}
	else
	{
		meleeMinion->SetState(ENormalMinionStates::Attack);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(ENormalMinionStates::Attack));
	}

	//OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, ENormalMinionStates::Attack);
	UE_LOG(LogTemp, Warning, TEXT("Call UpdateMeleeMinionState"));
}
