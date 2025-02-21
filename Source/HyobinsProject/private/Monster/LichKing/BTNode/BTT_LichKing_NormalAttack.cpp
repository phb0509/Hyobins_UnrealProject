// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_NormalAttack.h"
#include "AIController.h"
#include "Monster/Monster.h"
#include "Utility/AnimInstanceBase.h"


UBTT_LichKing_NormalAttack::UBTT_LichKing_NormalAttack()
{
}

EBTNodeResult::Type UBTT_LichKing_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	if (!m_bHasInit	)
	{
		m_bHasInit = true;
		
		animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("NormalAttack"),
	[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	}
	
	animInstance->PlayMontage(TEXT("NormalAttack"));
	
	return EBTNodeResult::InProgress;
}
