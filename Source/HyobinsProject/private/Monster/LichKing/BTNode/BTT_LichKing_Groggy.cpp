// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_Groggy.h"
#include "AIController.h"
#include "Monster/Monster.h"
#include "Component/StatComponent.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/EnumTypes.h"

UBTT_LichKing_Groggy::UBTT_LichKing_Groggy() :
	m_GroggyTime(4.0f)
{
}

EBTNodeResult::Type UBTT_LichKing_Groggy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(owner->GetMesh()->GetAnimInstance());
	
	if (!m_bHasInit	)
	{
		m_bHasInit = true;
		
		animInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Groggy"),
	[&,owner]()
		{
			owner->GetStatComponent()->SetStamina(100.0f);
			owner->SetFSMState(ELichKingFSMStates::Chase);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	}

	owner->SetIsSuperArmor(true);
	animInstance->PlayMontage(TEXT("Groggy"));

	FTimerHandle timerHandle;
	owner->GetWorldTimerManager().SetTimer
		(
			timerHandle,
			[=]()
			{ animInstance->StopAllMontages(0.1f); },
		m_GroggyTime,
		false);

	
	return EBTNodeResult::InProgress;
	
}
