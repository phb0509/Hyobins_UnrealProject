// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTS_FindPatrolPos.h"
#include "Monster/Monster.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTS_FindPatrolPos::UBTS_FindPatrolPos()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("FindPatrolPos");
}

void UBTS_FindPatrolPos::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	checkf(IsValid(monster), TEXT("monster is not Valid"));

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(monster->GetWorld());

	checkf(IsValid(NavSystem), TEXT("NavSystem is not Valid"));

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMeleeMinionAIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, monster->GetPatrolRange(), NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMeleeMinionAIController::PatrolPosKey, NextPatrol.Location);
	}

	GEngine->AddOnScreenDebugMessage(10, 3.f, FColor::Green, FString::Printf(TEXT("nextPatrol: %f  %f  %f"), NextPatrol.Location.X, NextPatrol.Location.Y, NextPatrol.Location.Z));
}