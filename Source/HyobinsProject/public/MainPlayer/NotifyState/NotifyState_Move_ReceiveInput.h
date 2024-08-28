// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_Move_ReceiveInput.generated.h"

class AMainPlayer;

UCLASS()
class HYOBINSPROJECT_API UNotifyState_Move_ReceiveInput : public UAnimNotifyState
{
	GENERATED_BODY()
	
	UNotifyState_Move_ReceiveInput();

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;


private:
	TWeakObjectPtr<AMainPlayer> m_Owner;
	
	UPROPERTY(EditAnywhere)
	float m_MoveOffset;

	int32 m_CurInputHorizontal;
	int32 m_CurInputVertical;
};
