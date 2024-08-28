// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_Move.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UNotifyState_Move : public UAnimNotifyState
{
	GENERATED_BODY()

	UNotifyState_Move();

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere)
	float m_MoveOffset;
	
	UPROPERTY(EditAnywhere)
	float m_HorizontalValue;
	
	UPROPERTY(EditAnywhere)
	float m_VerticalValue;
};
