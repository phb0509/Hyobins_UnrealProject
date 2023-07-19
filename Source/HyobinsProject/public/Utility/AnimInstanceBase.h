// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnDeathMontageEndedDelegate);

UCLASS()
class HYOBINSPROJECT_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstanceBase();

	void PlayMontage(FName montageName, float inPlayRate = 1.0f);
	void JumpToMontageSection(FName montageName, int32 newSection);
	UAnimMontage* GetMontage(FName montageName);

private:
	UFUNCTION()
		void AnimNotify_OnDeathMontageEnded(); // Notify를 실행시키는 함수.

public:
	FOnDeathMontageEndedDelegate OnDeathMontageEnded;

protected:
	TMap<FName, UAnimMontage*> m_Montages;
	
};
