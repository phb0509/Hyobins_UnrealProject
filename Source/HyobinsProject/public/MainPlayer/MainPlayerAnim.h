// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Animation/AnimInstance.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


DECLARE_MULTICAST_DELEGATE(FOnNextNormalAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNormalAttackHitCheckDelegate);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayNormalAttackMontage();
	void JumpToNormalAttackMontageSection(int32 newSection);

private:
	UFUNCTION()
		void AnimNotify_checkNormalAttackHit(); // Notify를 실행시키는 함수.

	UFUNCTION()
		void AnimNotify_checkNextNormalAttack();

	FName GetNormalAttackMontageSectionName(int32 section);


public:
	// 델리게이트.
	FOnNextNormalAttackCheckDelegate OnNextNormalAttackCheck;
	FOnNormalAttackHitCheckDelegate  OnNormalAttackHitCheck;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* m_NormalAttackMontage;

private:
	TWeakObjectPtr<class AMainPlayer> m_Owner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		bool m_bIsPressingShift;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass, Meta = (AllowPrivateAccess = true))
		bool m_bIsCombated;
};
