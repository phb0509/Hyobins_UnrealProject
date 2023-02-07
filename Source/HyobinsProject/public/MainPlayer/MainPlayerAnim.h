// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);


UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayNormalAttackMontage();
	void JumpToNormalAttackMontageSection(int32 newSection);

private:
	UFUNCTION()
		void AnimNotify_checkAttackHit(); // Notify를 실행시키는 함수.

	UFUNCTION()
		void AnimNotify_checkNextAttack();

	FName GetNormalAttackMontageSectionName(int32 section);

public:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
	//	EMainPlayerStates m_CurState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		float m_CurSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsPressingShift;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsCombated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsWalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsRunning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		bool m_bIsHit;

	// 델리게이트.
	FOnNextAttackCheckDelegate OnNextAttackCheck; 
	FOnAttackHitCheckDelegate  OnAttackHitCheck;

private:
	TWeakObjectPtr<class AMainPlayer> m_AMainPlayer;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* m_NormalAttackMontage;

};
