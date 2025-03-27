// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/BTNode/BTT_SuperMinion_NormalAttack.h"
#include "CharacterBase/CharacterBase.h"
#include "CharacterBase/AIControllerBase.h"
#include "CharacterBase/AnimInstanceBase.h"


UBTT_SuperMinion_NormalAttack::UBTT_SuperMinion_NormalAttack()
{}

EBTNodeResult::Type UBTT_SuperMinion_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACharacterBase* owner = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	int32 attackIndex = FMath::RandRange(0,1);
	FName attackName = FName("NormalAttack" + FString::FromInt(attackIndex));
	animInstance->PlayMontage(attackName);
	
	return EBTNodeResult::InProgress;
}

void UBTT_SuperMinion_NormalAttack::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	ACharacterBase* owner = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (owner != nullptr)
	{
		FInstanceNode* instanceNode = reinterpret_cast<FInstanceNode*>(NodeMemory);
		if (!instanceNode->bHasInit)
		{
			instanceNode->bHasInit = true;

			UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
        
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
	}
	
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
}


