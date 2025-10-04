// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "SkillComponent.generated.h"

class UAnimInstanceBase;
class APlayableCharacter;
class USkill;


USTRUCT(Atomic) 
struct FSkillClassList // Ư�� ��ǲ�������ؽ�Ʈ�� �����ִ� ��ų��. ex) InAir���� ��ų��..
{
	GENERATED_USTRUCT_BODY() 

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<USkill>> skillClassList;
};


USTRUCT(Atomic) 
struct FSkillList 
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
	
	void ExecuteSkill(const FName& inputMappingContextName, const FName& skillName);
	void InitGravityScaleAfterAttack(); // Ư�����ݵ�(���߿� ������Ű������ �߷°��� ���ϰ� ��������) ���� �ٽ� �������� �ʱ�ȭ.
	
	FORCEINLINE float GetGravityScaleInAir() const { return m_GravityScaleInAir; }
	const TMap<FName, FSkillList>* GetSkillList() const { return &m_SkillList; }
	FName GetHighestPriorityInputMappingContext() const { return m_Owner->GetHighestPriorityInputMappingContext(); }
	USkill* GetCurExecutingSkill() const { return m_CurSkill.Get(); }


	template <typename T> 
	T GetCurSkillStateEnum(const uint8 enumInteger) const
	{
		return static_cast<T>(enumInteger);
	}
	
	template<typename T>
	typename TEnableIf<(TIsEnumClass<T>::Value || TIsIntegral<T>::Value), bool>::Type
	IsCurSkillState(const T state) const
	{
		return m_CurSkillState == static_cast<uint8>(state);
	}

	template<typename T>
	typename TEnableIf<(TIsEnumClass<T>::Value || TIsIntegral<T>::Value), void>::Type
	SetSkillState(const T state)
	{
		m_CurSkillState = static_cast<uint8>(state);
	}

protected:
	bool HasSkill(const FName& inputMappingContextName, const FName& skillName) const;

private:
	void loadSkills();

	
protected:
	TWeakObjectPtr<APlayableCharacter> m_Owner;
	TWeakObjectPtr<UAnimInstanceBase> m_OwnerAnimInstance;
	
	uint8 m_CurSkillState;
	TWeakObjectPtr<USkill> m_CurSkill;
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillList")
	TMap<FName, FSkillClassList> m_SkillClassList; // Key : InputMappingContextName
	
	UPROPERTY(VisibleAnywhere, Transient, Category = "SkillList")
	TMap<FName, FSkillList> m_SkillList;

	UPROPERTY(EditAnywhere, Category = "GravityScaleInAir")
	float m_GravityScaleInAir;
};
