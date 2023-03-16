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

	void PlayNormalAttackMontage();



public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* m_NormalAttackMontage;

private:
	
	TWeakObjectPtr<class AMeleeMinion> m_Owner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		float m_CurSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		bool m_bIsIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		bool m_bIsWalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		bool m_bIsInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		bool m_bIsAttacking;
};
