// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/BTNode/BTT_LichKing_UpdateNormalPattern.h"
#include "AIController.h"
#include "Monster/Monster.h"
#include "Utility/EnumTypes.h"

UBTT_LichKing_UpdateNormalPattern::UBTT_LichKing_UpdateNormalPattern()
{
}

EBTNodeResult::Type UBTT_LichKing_UpdateNormalPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* owner = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	check(owner != nullptr);
	
	const int32 randomValue = FMath::RandRange(0,2);
	
	switch (randomValue)
	{
	case 0:
		owner->SetFSMState(ELichKingFSMStates::NormalAttack);
		break;
	
	case 1:
		owner->SetFSMState(ELichKingFSMStates::CrashAttack);
		break;
	
	case 2:
		owner->SetFSMState(ELichKingFSMStates::SoulSiphon);
		break;
	
	default:
		break;
	}
	
	return EBTNodeResult::Succeeded;
}
