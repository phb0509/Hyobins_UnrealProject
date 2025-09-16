// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "NotifyState_MotionWarping_Move.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UNotifyState_MotionWarping_Move : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UNotifyState_MotionWarping_Move(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	
private:
	UPROPERTY(EditAnywhere, Category = "Velocity")
	int8 m_Forward;

	UPROPERTY(EditAnywhere, Category = "Velocity")
	int8 m_Right;

	UPROPERTY(EditAnywhere, Category = "Velocity")
	int8 m_Up;
	
	UPROPERTY(EditAnywhere, Category = "Velocity")
	float m_MoveDistance;
};
