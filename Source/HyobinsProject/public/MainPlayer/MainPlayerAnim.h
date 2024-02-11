// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AnimInstanceBase.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;


DECLARE_MULTICAST_DELEGATE(FOnNormalAttackNextCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNormalAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndedNormalAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndedDodgeMoveDelegate);



UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();

private:
	void initAssets();

	// 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
	UFUNCTION()
		void AnimNotify_checkNormalAttackHit(); // 충돌체크해야할 타이밍에 바인딩된 노티파이

	UFUNCTION()
		void AnimNotify_checkNextNormalAttack(); // 추가키입력 검사타이밍에 바인딩된 노티파이

	UFUNCTION()
		void AnimNotify_EndedNormalAttack(); // 각 콤보모션 끝나는 타이밍에 바인딩된 노티파이

	UFUNCTION()
		void AnimNotify_EndedDodgeMove(); // 회피기모션 끝나는 타이밍에 바인딩된 노티파이


public:
	FOnNormalAttackNextCheckDelegate OnNormalAttackNextCheck;
	FOnNormalAttackHitCheckDelegate  OnNormalAttackHitCheck;
	FOnEndedNormalAttackDelegate OnEndedNormalAttack;
	FOnEndedDodgeMoveDelegate OnEndedDodgeMove;

};
