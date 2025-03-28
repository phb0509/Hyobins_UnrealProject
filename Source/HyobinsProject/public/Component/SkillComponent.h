// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

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

	void ExecEvent_AllSkillEnded();
	
	const TMap<FName, FSkillList>* GetSkillList() const { return &m_SkillList; }
	virtual FName GetHighestPriorityInputMappingContext() PURE_VIRTUAL(USkillComponent::Interact, return "Empty"; );
	TWeakObjectPtr<USkill> GetCurSkill() const { return m_CurSkill; }

protected:
	void ExecuteSkill(const FName& inputMappingContextName, const FName& skillName);
	
	FORCEINLINE bool HasSkill(const FName& inputMappingContextName, const FName& skillName) const
	{
		return m_SkillList.Contains(inputMappingContextName) && m_SkillList[inputMappingContextName].skillList.Contains(skillName) &&
			m_SkillList[inputMappingContextName].skillList[skillName] != nullptr;
	}

private:
	void loadSkills();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SkillList")
	TMap<FName, FSkillClassList> m_SkillClassList; // Key : InputMappingContextName
	
	UPROPERTY(VisibleAnywhere, Transient, Category = "SkillList")
	TMap<FName, FSkillList> m_SkillList;

	TWeakObjectPtr<USkill> m_CurSkill;
};
