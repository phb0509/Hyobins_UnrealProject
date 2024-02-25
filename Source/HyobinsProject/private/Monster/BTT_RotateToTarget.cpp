// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTT_RotateToTarget.h"
#include "Utility/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Monster.h"

UBTT_RotateToTarget::UBTT_RotateToTarget()
{
	NodeName = TEXT("RotateToTarget");
}

EBTNodeResult::Type UBTT_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* const owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	const ACharacterBase* const enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));

	const FVector toTargetVector = enemyOnBlackBoard->GetActorLocation() - owner->GetActorLocation();
	const FRotator LookAtRotation = FRotator(0.f, toTargetVector.Rotation().Yaw, 0.f);
	owner->SetActorRotation(LookAtRotation);

	return EBTNodeResult::Succeeded;
}