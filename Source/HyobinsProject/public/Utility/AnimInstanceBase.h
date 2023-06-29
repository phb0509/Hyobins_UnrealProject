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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayMontage(FName montageName, float inPlayRate = 1.0f);
	void JumpToMontageSection(FName montageName, int32 newSection);

private:
	UFUNCTION()
		void AnimNotify_OnDeathMontageEnded(); // Notify를 실행시키는 함수.

public:
	FOnDeathMontageEndedDelegate OnDeathMontageEnded;

protected:
	TWeakObjectPtr<class ACharacterBase> m_Owner;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	float m_CurSpeed;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	bool m_bIsIdle;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	bool m_bIsWalking;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	bool m_bIsRunning;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	bool m_bIsInAir;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	bool m_bIsDeath;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimClass)
	//	int m_DeathSequenceIndex;


	TMap<FName, UAnimMontage*> m_Montages;
	

};
