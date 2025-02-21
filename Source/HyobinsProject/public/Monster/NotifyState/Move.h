// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Move.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UMove : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UMove();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(Editanywhere)
	float m_MoveDistance;
	
	FVector m_StartLocation; // 시작 위치 저장
	FVector m_TargetLocation;
	
	float m_Duration;
	float m_ElapsedTime;
	float m_DistancePerSecond; // 초당 이동 거리
};
