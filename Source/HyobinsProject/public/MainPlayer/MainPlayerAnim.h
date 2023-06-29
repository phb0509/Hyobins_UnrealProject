// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


DECLARE_MULTICAST_DELEGATE(FOnNormalAttackNextCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNormalAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndedDodgeMoveDelegate);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	void initAssets();

	UFUNCTION()
		void AnimNotify_checkNormalAttackHit();

	UFUNCTION()
		void AnimNotify_checkNextNormalAttack();

	UFUNCTION()
		void AnimNotify_EndedDodgeMove();


public:
	FOnNormalAttackNextCheckDelegate OnNormalAttackNextCheck;
	FOnNormalAttackHitCheckDelegate  OnNormalAttackHitCheck;
	FOnEndedDodgeMoveDelegate OnEndedDodgeMove;


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class AMainPlayer> m_Owner;


};
