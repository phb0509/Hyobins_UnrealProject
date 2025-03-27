// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_Groggy.h"
#include "AIController.h"
#include "Monster/Monster.h"
#include "Component/StatComponent.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Utility/EnumTypes.h"

UBTT_LichKing_Groggy::UBTT_LichKing_Groggy() :
	m_GroggyTime(4.0f)
{
}

EBTNodeResult::Type UBTT_LichKing_Groggy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACharacterBase* owner = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	owner->SetIsSuperArmor(true);
	owner->GetStatComponent()->SetCanRecoveryHP(false);
	owner->GetStatComponent()->SetCanRecoveryStamina(false);
	animInstance->PlayMontage(TEXT("Groggy"));

	FTimerHandle timerHandle;
	owner->GetWorldTimerManager().SetTimer
		(
			timerHandle,
			[=]()
			{ 
				animInstance->StopAllMontages(0.0f); 
			},
		m_GroggyTime,
		false);

	
	return EBTNodeResult::InProgress;
}

void UBTT_LichKing_Groggy::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (owner != nullptr)
	{
		FInstanceNode* instanceNode = reinterpret_cast<FInstanceNode*>(NodeMemory);

		if (!instanceNode->bHasInit)
		{
			instanceNode->bHasInit = true;
			
			UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
		
			animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Groggy"),
		[&,owner]()
			{
				if (!owner->GetIsDead())
				{
					owner->GetStatComponent()->SetStaminaPercent(100.0f);
					owner->GetStatComponent()->SetCanRecoveryHP(true);
					owner->GetStatComponent()->SetCanRecoveryStamina(true);
					owner->SetFSMState(ELichKingFSMStates::Chase);
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});
		}
	}
	
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
}