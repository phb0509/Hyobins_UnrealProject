// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTS_FindPatrolPos.h"
#include "Monster/Monster.h"
#include "Utility/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTS_FindPatrolPos::UBTS_FindPatrolPos() :
m_PatrolRange(500.0f)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("FindPatrolPos");
}

void UBTS_FindPatrolPos::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const AMonster* const owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	const UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetNavigationSystem(owner->GetWorld());
	
	const FVector origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonster::HomePosKey);
	FNavLocation nextPatrol;  
	
	if (NavSystem->GetRandomPointInNavigableRadius(origin, m_PatrolRange, nextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonster::PatrolPosKey, nextPatrol.Location);
	}
}
