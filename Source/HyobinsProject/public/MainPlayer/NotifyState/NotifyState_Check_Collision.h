// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_Check_Collision.generated.h"

class ACharacterBase;
class UDataManager;
class USoundWave;

UCLASS()
class HYOBINSPROJECT_API UNotifyState_Check_Collision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UNotifyState_Check_Collision();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	
	UPROPERTY(EditAnywhere)
	FName m_ColliderName;

	UPROPERTY(EditAnywhere)
	FName m_AttackName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundWave> m_HitSound;
};