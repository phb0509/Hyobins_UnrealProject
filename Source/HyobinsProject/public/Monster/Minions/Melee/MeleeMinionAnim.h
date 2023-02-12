// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MeleeMinionAnim.generated.h"

UCLASS()
class HYOBINSPROJECT_API UMeleeMinionAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMeleeMinionAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	TWeakObjectPtr<class AMeleeMinion> m_AMainPlayer;
};
