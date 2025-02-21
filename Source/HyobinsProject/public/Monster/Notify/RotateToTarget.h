// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API URotateToTarget : public UAnimNotify
{
	GENERATED_BODY()

public:
	URotateToTarget();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FRotator m_RotatorOffset;
};
