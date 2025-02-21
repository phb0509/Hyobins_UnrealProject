// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_SoulSiphon.h"
#include "AIController.h"
#include "Monster/Monster.h"
#include "Utility/AnimInstanceBase.h"


UBTT_LichKing_SoulSiphon::UBTT_LichKing_SoulSiphon()
{
}

EBTNodeResult::Type UBTT_LichKing_SoulSiphon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	if (!m_bHasInit)
	{
		m_bHasInit = true;
		
		animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("SoulSiphon"),
	[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	}
	
	animInstance->PlayMontage(TEXT("SoulSiphon"));
	
	return EBTNodeResult::InProgress;
}
