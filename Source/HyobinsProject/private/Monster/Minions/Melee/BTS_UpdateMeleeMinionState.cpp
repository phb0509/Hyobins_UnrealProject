// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/BTS_UpdateMeleeMinionState.h"



UBTS_UpdateMeleeMinionState::UBTS_UpdateMeleeMinionState()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("UpdateMeleeMinionState");
}

void UBTS_UpdateMeleeMinionState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

}
