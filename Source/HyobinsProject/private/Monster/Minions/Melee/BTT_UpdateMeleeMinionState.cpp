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

	AMeleeMinion* meleeMinion = Cast<AMeleeMinion>(OwnerComp.GetAIOwner()->GetPawn());
	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Enemy"));


	
	float distanceToEnemy = meleeMinion->GetDistanceTo(enemyOnBlackBoard);

	if (distanceToEnemy > meleeMinion->GetNormalAttackRange()) // 공격범위 밖이면
	{
		meleeMinion->SetState(ENormalMinionStates::Chase); // 공격할 수 있는 거리 될 때까지 추적.
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(ENormalMinionStates::Chase));
		UE_LOG(LogTemp, Warning, TEXT(" ChaseState!!!!!!!!!!!!!!!"));
		UE_LOG(LogTemp, Warning, TEXT(" Chase :: distance To Enemy : %f"), distanceToEnemy);
	}
	else if (distanceToEnemy <= meleeMinion->GetNormalAttackRange())
	{
		meleeMinion->SetState(ENormalMinionStates::Attack);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(ENormalMinionStates::Attack));
		UE_LOG(LogTemp, Warning, TEXT(" AttacState!!!!!!!!!!!!!!!"));
		UE_LOG(LogTemp, Warning, TEXT(" Attack :: distance To Enemy : %f"), distanceToEnemy);
	}

	//OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, ENormalMinionStates::Attack);
	//UE_LOG(LogTemp, Warning, TEXT("Call UpdateMeleeMinionState"));


	return EBTNodeResult::Succeeded;
}
