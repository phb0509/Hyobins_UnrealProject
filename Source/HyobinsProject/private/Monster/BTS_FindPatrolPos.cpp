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

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector("HomePos");
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, monster->GetPatrolRange(), NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolPos", NextPatrol.Location);
	}
	UE_LOG(LogTemp, Warning, TEXT("Call FindPath"));
}
