// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomStructs.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UStatComponent;
class AAIControllerBase;
class UAIPerceptionComponent;
class UShapeComponent;

enum class ECrowdControlType : uint8;
enum class EMonsterCommonStates : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCrowdControl_Delegate, ACharacterBase*);

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void OnCalledTimer_Knockback_End() {};

	UFUNCTION()
	virtual void OnCalledNotify_End_Death(); 

	void Attack(const FName& attackName, TWeakObjectPtr<AActor> target);
	
	// Get
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE UAIPerceptionComponent* GetAIPerceptionComponent() const { return m_AIPerceptionComponent; }
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsDead() const { return m_bIsDead; }
	FORCEINLINE int32 GetHitDirection() const { return m_HitDirection; }
	FORCEINLINE UShapeComponent* GetCollider(const FName& colliderName) { return m_Colliders[colliderName].Get(); }
	
	
	bool HasContainHitActor(const FName& attackName, AActor* hitActor)
	{
		return m_HitActorsByMe[attackName].Contains(hitActor);
	}
	
	void AddCheckedHitActor(const FName& attackName, AActor* hitActor)
	{
		m_HitActorsByMe[attackName].Add(hitActor,true);
	}

	void EmptyCheckedHitActor(const FName& attackName)
	{
		m_HitActorsByMe[attackName].Empty();
	}

	
protected:
	//UFUNCTION()
	virtual void ExecEvent_Knockback(ACharacterBase* instigator){};
	//UFUNCTION()
	virtual void ExecEvent_Groggy(ACharacterBase* instigator){};
	virtual void ExecEvent_OnHPIsZero();
	virtual void Die() {};
	virtual void ExecEvent_EndedDeathMontage() {}; 

	

protected:
	TMap<FName, TMap<TWeakObjectPtr<AActor>, bool>> m_HitActorsByMe;
	
	TMap<FName, TWeakObjectPtr<UShapeComponent>> m_Colliders;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStatComponent* m_StatComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UAIPerceptionComponent* m_AIPerceptionComponent;

	TMap<ECrowdControlType, FOnCrowdControl_Delegate> m_CrowdControlDelegates;
	
	FTimerHandle m_CrowdControlTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	float m_CrowdControlTime;
	
	FTimerHandle m_DeathTimerHandle;


	
	UPROPERTY(EditDefaultsOnly)
	float m_DeathTimerTime;
	
	FTimerHandle m_DeActivateTimerHandle;
	
	UPROPERTY(EditDefaultsOnly) 
	float m_WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float m_RunSpeed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	float m_CurSpeed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsSuperArmor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsDead;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	int32 m_HitDirection;

};
