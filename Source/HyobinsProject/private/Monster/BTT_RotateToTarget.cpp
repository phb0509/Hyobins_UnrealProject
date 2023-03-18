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

	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	ACharacterBase* enemyOnBlackBoard = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::EnemyKey));

	FVector ToTarget = enemyOnBlackBoard->GetActorLocation() - owner->GetActorLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	owner->SetActorRotation(LookAtRotation);

	return EBTNodeResult::Succeeded;
}