// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnEndedDeathDelegate);

UCLASS()
class HYOBINSPROJECT_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstanceBase();

	void PlayMontage(const FName& montageName, float inPlayRate = 1.0f);
	void JumpToMontageSection(const FName& montageName, int32 newSection);
	UAnimMontage* GetMontage(const FName& montageName);

private:
	UFUNCTION()
	void AnimNotify_Pause();
	
	UFUNCTION()
	void AnimNotify_EndedDeath() const; 

public:
	FOnEndedDeathDelegate OnEndedDeath;

protected:
	UPROPERTY()
	TMap<FName, UAnimMontage*> m_Montages;
	
};
