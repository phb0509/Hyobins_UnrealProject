// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTNode/BTT_SuperMinion_NormalAttack.h"
#include "Monster/Monster.h"
#include "Utility/AIControllerBase.h"
#include "Utility/AnimInstanceBase.h"


UBTT_SuperMinion_NormalAttack::UBTT_SuperMinion_NormalAttack()
{}

EBTNodeResult::Type UBTT_SuperMinion_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	FInstanceNode* instanceNode = reinterpret_cast<FInstanceNode*>(NodeMemory);

	if (!instanceNode->bHasInit)
	{
		instanceNode->bHasInit = true;
		
		animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack0"),
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

		animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack1"),
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	}
	
	int32 attackIndex = FMath::RandRange(0,1);
	FName attackName = FName("NormalAttack" + FString::FromInt(attackIndex));
	animInstance->PlayMontage(attackName);
	
	return EBTNodeResult::InProgress;
}


