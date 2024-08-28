// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_SuperMinion_NormalAttack.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Utility/AIControllerBase.h"
#include "Utility/AnimInstanceBase.h"

static FName NormalAttackNames[2] = {"NormalAttack0", "NormalAttack1"};

UBTT_SuperMinion_NormalAttack::UBTT_SuperMinion_NormalAttack()
{
	NodeName = TEXT("SuperMinion_NormalAttack");
}

EBTNodeResult::Type UBTT_SuperMinion_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASuperMinion* const owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
	AAIControllerBase* aiController = Cast<AAIControllerBase>(owner->GetController());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	aiController->StopBehaviorTree();
	animInstance->PlayMontage(NormalAttackNames[FMath::RandRange(0,1)],1.0f);
	
	return EBTNodeResult::Succeeded;
}
