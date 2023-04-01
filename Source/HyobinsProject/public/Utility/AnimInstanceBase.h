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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	TArray<UAnimMontage*> GetAttackMontages() { return m_AttackMontages; }
	TArray<UAnimMontage*> GetOnHitMontages() { return m_OnHitMontages; }
	TArray<UAnimSequence*> GetDeathSequences() { return m_DeathSequences; }

	void SetDeathSequenceIndex(int index) { m_DeathSequenceIndex = index; }

	

protected:
	TWeakObjectPtr<class ACharacterBase> m_Owner;

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
		bool m_bIsDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		int m_DeathSequenceIndex;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		TArray<UAnimMontage*> m_AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		TArray<UAnimMontage*> m_OnHitMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
		TArray<UAnimSequence*> m_DeathSequences;
};
