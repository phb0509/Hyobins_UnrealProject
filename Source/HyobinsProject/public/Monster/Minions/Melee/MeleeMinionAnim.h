// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Animation/AnimInstance.h"
#include "Utility/AnimInstanceBase.h"
#include "MeleeMinionAnim.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class HYOBINSPROJECT_API UMeleeMinionAnim : public UAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	UMeleeMinionAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayNormalAttackMontage();



private:
	void loadMontages();

	UFUNCTION()
		void AnimNotify_checkAttackHit(); // Notify�� �����Ű�� �Լ�.

public:
	FOnAttackHitCheckDelegate  OnAttackHitCheck;

private:
	TWeakObjectPtr<class AMeleeMinion> m_Owner;


};
