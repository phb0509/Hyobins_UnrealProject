// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetFSMState.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UBTS_SetFSMState : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_SetFSMState();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	FORCEINLINE FText GetFSMStateName() const { return FText::FromString(m_SelectedFSMStateName); } 
	FORCEINLINE void SetFSMStateName(FString stateName) { m_SelectedFSMStateName = stateName; }

	FORCEINLINE uint8 GetFSMStateIndex() const { return m_FSMStateIndex; } 
	FORCEINLINE void SetFSMStateIndex(const uint8 stateName)
	{
		m_FSMStateIndex = stateName;
		this->Modify();
	}

	FORCEINLINE FText GetEnumClassName() const { return FText::FromString(m_EnumClassName); }
	FORCEINLINE void SetEnumClassName(FString enumClassName) { m_EnumClassName = enumClassName; }

private:
	UPROPERTY(VisibleAnywhere, meta = (HideInDetailPanel))
	FString m_EnumClassName;
	
	UPROPERTY(VisibleAnywhere, meta = (HideInDetailPanel))
	FString m_SelectedFSMStateName;

	UPROPERTY(VisibleAnywhere, meta = (HideInDetailPanel))
	uint8 m_FSMStateIndex;
};
