// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_CrashAttack.h"
#include "AIController.h"
#include "Monster/Monster.h"
#include "Utility/AnimInstanceBase.h"

UBTT_LichKing_CrashAttack::UBTT_LichKing_CrashAttack()
{
}

EBTNodeResult::Type UBTT_LichKing_CrashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	FInstanceNode* instanceNode = reinterpret_cast<FInstanceNode*>(NodeMemory);

	if (!instanceNode->bHasInit)
	{
		instanceNode->bHasInit = true;
		
		animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("CrashAttack"),
	[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	}

	animInstance->PlayMontage(TEXT("CrashAttack"));
	
	return EBTNodeResult::InProgress;
}
