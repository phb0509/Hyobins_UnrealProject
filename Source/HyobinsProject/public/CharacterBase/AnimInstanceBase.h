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
	FSkillMontageEvent_Delegate funcOnCalledMontageNotInterruptedEnded;
	FSkillMontageEvent_Delegate funcOnCalledMontageInterruptedEnded;
	FSkillMontageEvent_Delegate funcOnCalledMontageAllEnded;
};

DECLARE_MULTICAST_DELEGATE(FOnEndedDeathDelegate);

UCLASS()
class HYOBINSPROJECT_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstanceBase();
	
	virtual void NativeBeginPlay() override;
	
	void PlayMontage(const FName& montageName, float inPlayRate = 1.0f);
	void JumpToMontageSectionByIndex(const UAnimMontage* montage, int32 newSection);
	void JumpToMontageSectionByName(const UAnimMontage* montage, FName newSection);
	bool IsCurrentMontage(const FName& montageName);
	FName GetCurrentMontageName() const;
	
	float GetMontagePlayTime(const UAnimMontage* montage);
	UAnimMontage* GetMontage(const FName& montageName);

	FORCEINLINE bool IsLastMontagePlayInterrupted() const { return m_bIsLastMontagePlayInterrupted; }
	


	// Bind Func
	template <typename UObjectTemplate>
	void BindFunc_OnMontageStarted(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.BindUFunction(InUserObject, InFunctionName);
		
	};

	template <typename UObjectTemplate>
	void BindFunc_OnMontageNotInterruptedEnded(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageNotInterruptedEnded.BindUFunction(InUserObject, InFunctionName);
	};

	template <typename UObjectTemplate>
	void BindFunc_OnMontageInterruptedEnded(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageInterruptedEnded.BindUFunction(InUserObject, InFunctionName);
	};

	template <typename UObjectTemplate>
	void BindFunc_OnMontageAllEnded(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageAllEnded.BindUFunction(InUserObject, InFunctionName);
	};


	// Bind LambdaFunc
	template <typename FunctorType>
	void BindLambdaFunc_OnMontageStarted(const FName& montageName, FunctorType&& InFunctor)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.BindLambda(InFunctor);
	};
	
	template <typename FunctorType>
	void BindLambdaFunc_OnMontageNotInterruptedEnded(const FName& montageName, FunctorType&& InFunctor)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageNotInterruptedEnded.BindLambda(InFunctor);
	};

	template <typename FunctorType>
	void BindLambdaFunc_OnMontageInterruptedEnded(const FName& montageName, FunctorType&& InFunctor)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageInterruptedEnded.BindLambda(InFunctor);
	};
	
	template <typename FunctorType>
	void BindLambdaFunc_OnMontageAllEnded(const FName& montageName, FunctorType&& InFunctor)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			const FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}

		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageAllEnded.BindLambda(InFunctor);
	};


	
private:
	UFUNCTION()
	virtual void Exec_OnMontageStarted(UAnimMontage* montage);
	
	UFUNCTION()
	virtual void Exec_OnMontageEnded(UAnimMontage* montage, bool bInterrupted);


private:
	UFUNCTION()
	void AnimNotify_Pause();
	
	UFUNCTION()
	void AnimNotify_End_Death() const;



public:
	FOnEndedDeathDelegate End_Death;

protected:
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UAnimMontage>> m_Montages;
	
	TMap<FName, FMontageFunc> m_FuncsOnCalledMontageEvent;

	bool m_bIsLastMontagePlayInterrupted;
};


