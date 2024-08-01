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
	
	virtual void NativeBeginPlay() override;
	
	void PlayMontage(const FName& montageName, float inPlayRate = 1.0f);
	void JumpToMontageSection(const FName& montageName, int32 newSection);
	UAnimMontage* GetMontage(const FName& montageName);
	
	template <typename UObjectTemplate>
	void BindFunc_OnMontageStarted(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.BindUFunction(InUserObject, InFunctionName);
	};

	template <typename UObjectTemplate>
	void BindFunc_OnMontageEnded(const FName& montageName, UObjectTemplate* InUserObject, const FName& InFunctionName)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageEnded.BindUFunction(InUserObject, InFunctionName);
	};

	template <typename FunctorType>
	void BindLambdaFunc_OnMontageStarted(const FName& montageName, FunctorType&& InFunctor)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageStarted.BindLambda(InFunctor);
	};
	
	template <typename FunctorType>
	void BindLambdaFunc_OnMontageEnded(const FName& montageName,  FunctorType&& InFunctor)
	{
		if (!m_FuncsOnCalledMontageEvent.Contains(montageName))
		{
			FMontageFunc montageFunc;
			m_FuncsOnCalledMontageEvent.Add(montageName, montageFunc);
		}
		
		m_FuncsOnCalledMontageEvent[montageName].funcOnCalledMontageEnded.BindLambda(InFunctor);
	};

	void ExecBindedFunc_OnMontageStarted(const FName& montageName);
	void ExecBindedFunc_OnMontageEnded(const FName& montageName);
	
	UFUNCTION()
	virtual void Exec_OnMontageStarted(UAnimMontage* Montage);
	
	UFUNCTION()
	virtual void Exec_OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);


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

	bool m_bAfterInitAttackInfo; // 연계공격몽타주용 변수. 다음 연계섹션으로 넘어가기전에 공격정보초기화가 안되는 경우를 위함.
								 // 이 변수를 사용한 몽타주는, Ended에서 반드시 false로 바꾸는 코드를 바인딩할 것.
};


