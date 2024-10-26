// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTS_SetFSMState.h"
#include "Monster/Monster.h"
#include "Utility/AIControllerBase.h"


UBTS_SetFSMState::UBTS_SetFSMState() :
	m_EnumClassName(""),
	m_SelectedFSMStateName(""),
	m_FSMStateIndex(0)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("SetFSMState");
}

void UBTS_SetFSMState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (owner != nullptr)
	{
		owner->SetFSMState(m_FSMStateIndex);
	}
}

