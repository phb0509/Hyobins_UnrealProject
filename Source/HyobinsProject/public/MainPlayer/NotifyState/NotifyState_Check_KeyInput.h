// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_Check_KeyInput.generated.h"

class UBoxComponent;

UCLASS()
class HYOBINSPROJECT_API UNotifyState_Check_KeyInput : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UNotifyState_Check_KeyInput();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


private:
	bool m_bIsPressedKey;

	
};
