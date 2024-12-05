// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkill;

USTRUCT(Atomic, BlueprintType) 
struct FSkillList
{
	GENERATED_USTRUCT_BODY() 

public:
	UPROPERTY()
	TMap<FName, TObjectPtr<USkill>> skillList;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HYOBINSPROJECT_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	const TMap<FName, FSkillList>* GetSkillList() const { return &m_SkillList; }
	virtual FName GetHighestPriorityInputMappingContext() PURE_VIRTUAL(USkillComponent::Interact, return ""; );
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TMap<FName, FSkillList> m_SkillList;
};
