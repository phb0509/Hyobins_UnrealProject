// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "Utility/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("ExecuteTask!!!"));

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControllingPawn is NULL!!!!!!"));
		return EBTNodeResult::Failed;
	}
		
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());

	if (NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is NULL!!!!!!"));
		return EBTNodeResult::Failed;
	}
		

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIControllerBase::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is NULL!!!!!!"));
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerBase::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
