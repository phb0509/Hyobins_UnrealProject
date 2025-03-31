// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class UAnimInstanceBase;
class APlayableCharacter;
class USkill;

USTRUCT(Atomic) 
struct FSkillClassList // 특정 인풋매핑컨텍스트에 속해있는 스킬들. ex) InAir만의 스킬들..
{
	GENERATED_USTRUCT_BODY() 

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<USkill>> skillClassList;
};


USTRUCT(Atomic) 
struct FSkillList // 특정 인풋매핑컨텍스트에 속해있는 스킬들. ex) InAir만의 스킬들..
{
	GENERATED_USTRUCT_BODY() 

public:
	UPROPERTY(VisibleAnywhere)
	TMap<FName, USkill*> skillList;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, AllowedClasses="APlayableCharacter"))
class HYOBINSPROJECT_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	virtual void BeginPlay() override;
	
	const TMap<FName, FSkillList>* GetSkillList() const { return &m_SkillList; }
	FName GetHighestPriorityInputMappingContext() const;
	TWeakObjectPtr<USkill> GetCurSkill() const { return m_CurSkill; }

	template<typename T>
	typename TEnableIf<(TIsEnumClass<T>::Value || TIsIntegral<T>::Value), bool>::Type
	IsCurSkillState(const T state) const
	{
		const uint8 enumIndex = static_cast<uint8>(state);
		return m_CurSkillState == enumIndex;
	}

	template<typename T>
	typename TEnableIf<(TIsEnumClass<T>::Value || TIsIntegral<T>::Value), void>::Type
	SetSkillState(const T state)
	{
		const uint8 enumIndex = static_cast<uint8>(state);
		m_CurSkillState = enumIndex;
	}

protected:
	void ExecuteSkill(const FName& inputMappingContextName, const FName& skillName);
	
	FORCEINLINE bool HasSkill(const FName& inputMappingContextName, const FName& skillName) const
	{
		return m_SkillList.Contains(inputMappingContextName) && m_SkillList[inputMappingContextName].skillList.Contains(skillName) &&
			m_SkillList[inputMappingContextName].skillList[skillName] != nullptr;
	}

private:
	void loadSkills();
	void initSkills();
	
protected:
	TWeakObjectPtr<APlayableCharacter> m_Owner;
	TWeakObjectPtr<UAnimInstanceBase> m_OwnerAnimInstance;
	
	uint8 m_CurSkillState;
	TWeakObjectPtr<USkill> m_CurSkill;
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillList")
	TMap<FName, FSkillClassList> m_SkillClassList; // Key : InputMappingContextName
	
	UPROPERTY(VisibleAnywhere, Transient, Category = "SkillList")
	TMap<FName, FSkillList> m_SkillList;

	
};
