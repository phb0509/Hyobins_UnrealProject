// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Notify_FindTarget.generated.h"

class ACharacterBase;

UCLASS()
class HYOBINSPROJECT_API UNotify_FindTarget : public UAnimNotify
{
	GENERATED_BODY()

public:
	UNotify_FindTarget();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere)
	float m_CollisionRadius;
};
