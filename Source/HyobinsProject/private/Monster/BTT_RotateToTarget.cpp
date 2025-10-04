// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTT_RotateToTarget.h"
#include "CharacterBase/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_RotateToTarget::UBTT_RotateToTarget()
{
	NodeName = TEXT("RotateToTarget");
}

EBTNodeResult::Type UBTT_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* const owner = OwnerComp.GetAIOwner()->GetPawn();
	check(owner != nullptr);
	
	const AActor* const enemyOnBlackBoard = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Enemy")));
	check(enemyOnBlackBoard != nullptr);
	
	const FVector toTargetVector = enemyOnBlackBoard->GetActorLocation() - owner->GetActorLocation();
	const FRotator LookAtRotation = FRotator(0.f, toTargetVector.Rotation().Yaw, 0.f);
	
	owner->SetActorRotation(LookAtRotation);

	return EBTNodeResult::Succeeded;
}