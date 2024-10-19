// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTT_SuperMinion_NormalAttack.h"
#include "Monster/Minions/Super/SuperMinion.h"
#include "Utility/AIControllerBase.h"
#include "Utility/AnimInstanceBase.h"


UBTT_SuperMinion_NormalAttack::UBTT_SuperMinion_NormalAttack()
{
	NodeName = TEXT("SuperMinion_NormalAttack");
}

EBTNodeResult::Type UBTT_SuperMinion_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	const ASuperMinion* owner = Cast<ASuperMinion>(OwnerComp.GetAIOwner()->GetPawn());
	AAIControllerBase* aiController = Cast<AAIControllerBase>(owner->GetController());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	int32 attackIndex = FMath::RandRange(0,1);
	FString attackName = "NormalAttack" + FString::FromInt(attackIndex);
	
	aiController->StopBehaviorTree();
	animInstance->PlayMontage(FName(attackName));
	
	return EBTNodeResult::Succeeded;
}
