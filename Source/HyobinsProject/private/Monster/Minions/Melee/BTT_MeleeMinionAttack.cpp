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
	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));

	FVector ToTarget = enemyOnBlackBoard->GetActorLocation() - owner->GetActorLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	owner->SetActorRotation(LookAtRotation);

	owner->NormalAttack();

	return EBTNodeResult::Succeeded;
}