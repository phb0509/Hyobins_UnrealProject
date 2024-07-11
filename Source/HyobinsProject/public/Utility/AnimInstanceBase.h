// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"


DECLARE_DELEGATE(FSkillMontageEvent_Delegate);

USTRUCT(Atomic) 
struct FMontageFunc 
{
	GENERATED_USTRUCT_BODY() 

public:
	FSkillMontageEvent_Delegate funcOnCalledMontageStarted;
	FSkillMontageEvent_Delegate funcOnCalledMontageEnded;
	FSkillMontageEvent_Delegate funcOnCalledMontageInterruptedEnded;
};

DECLARE_MULTICAST_DELEGATE(FOnEndedDeathDelegate);

UCLASS()
class HYOBINSPROJECT_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstanceBase();
	
	void NativeBeginPlay() override;
	
	void PlayMontage(const FName& montageName, float inPlayRate = 1.0f);
	void JumpToMontageSection(const FName& montageName, int32 newSection);
	UAnimMontage* GetMontage(const FName& montageName);
	
	template <typename UObjectTemplate>
	void BindFuncOnMontageStarted(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.BindUFunction(InUserObject, InFunctionName);
	};

	template <typename UObjectTemplate>
	void BindFuncOnMontageEnded(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageEnded.BindUFunction(InUserObject, InFunctionName);
	};

	void ExecFuncOnMontageStarted(const FName& montageName);
	void ExecFuncOnMontageEnded(const FName& montageName);

public:
	UFUNCTION()
	virtual void onMontageStarted(UAnimMontage* Montage);
	
	UFUNCTION()
	virtual void onMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UFUNCTION()
	void AnimNotify_Pause();
	
	UFUNCTION()
	void AnimNotify_End_Death() const; 

public:
	FOnEndedDeathDelegate End_Death;

protected:
	UPROPERTY()
	TMap<FName, UAnimMontage*> m_Montages;
	TMap<FName, FMontageFunc> m_FuncsOnCalledMontageEvent;
	
};


