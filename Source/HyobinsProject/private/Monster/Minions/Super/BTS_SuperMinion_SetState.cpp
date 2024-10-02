// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTS_SuperMinion_SetState.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Utility/EnumTypes.h"

UBTS_SuperMinion_SetState::UBTS_SuperMinion_SetState() :
m_State(ESuperMinionFSMStates::Patrol)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("SuperMinion_SetState");
}

void UBTS_SuperMinion_SetState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	ASuperMinion* const owner = Cast<ASuperMinion>(OwnerComp.GetOwner());
	
	if (owner != nullptr)
	{
		owner->SetFSMState(m_State); 
	}
}
