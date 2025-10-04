// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_SoulSiphon.h"
#include "AIController.h"
#include "CharacterBase/CharacterBase.h"
#include "CharacterBase/AnimInstanceBase.h"


UBTT_LichKing_SoulSiphon::UBTT_LichKing_SoulSiphon()
{
}

EBTNodeResult::Type UBTT_LichKing_SoulSiphon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACharacterBase* owner = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	check(owner != nullptr);
	
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	check(animInstance != nullptr);
	
	animInstance->PlayMontage(TEXT("SoulSiphon"));
	
	return EBTNodeResult::InProgress;
}

void UBTT_LichKing_SoulSiphon::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
	
	ACharacterBase* owner = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (owner != nullptr)
	{
		FInstanceNode* instanceNode = reinterpret_cast<FInstanceNode*>(NodeMemory);

		if (!instanceNode->bHasInit)
		{
			instanceNode->bHasInit = true;

			UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
			check(animInstance != nullptr);
			
			animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("SoulSiphon"),
		[&]()
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});
		}
	}
}