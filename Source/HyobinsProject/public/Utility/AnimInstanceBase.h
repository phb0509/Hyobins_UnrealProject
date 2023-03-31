// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstanceBase();
	
	virtual void PlayOnHitMontage(int index);
	virtual void PlayDeathMontage(int index);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		float m_CurSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		bool m_bIsIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		bool m_bIsWalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		bool m_bIsRunning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		bool m_bIsInAir;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		TArray<UAnimMontage*> m_AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		TArray<UAnimMontage*> m_OnHitMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		TArray<UAnimMontage*> m_DeathMontages;
};
