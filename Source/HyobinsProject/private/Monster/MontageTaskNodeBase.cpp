// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MontageTaskNodeBase.h"

UMontageTaskNodeBase::UMontageTaskNodeBase()
{
}

void UMontageTaskNodeBase::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
}


