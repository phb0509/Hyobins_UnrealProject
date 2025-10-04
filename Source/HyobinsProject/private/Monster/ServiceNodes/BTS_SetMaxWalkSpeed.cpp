// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/ServiceNodes/BTS_SetMaxWalkSpeed.h"
#include "AIController.h"
#include "CharacterBase/CharacterBase.h"

UBTS_SetMaxWalkSpeed::UBTS_SetMaxWalkSpeed() :
	m_MaxWalkSpeed(600.0f)
{
	bNotifyBecomeRelevant = true;
}

void UBTS_SetMaxWalkSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	ACharacterBase* owner = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	check(owner != nullptr);
	
	owner->GetCharacterMovement()->MaxWalkSpeed = m_MaxWalkSpeed;
}
