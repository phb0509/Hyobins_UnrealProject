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

enum class EMonsterCommonStates : uint8;

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	
	virtual void OnCalledTimer_EndedOnHitKnockback() {};

	UFUNCTION()
	virtual void OnCalledNotify_End_Death(); 
	
	// Get
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE UAIPerceptionComponent* GetAIPerceptionComponent() const { return m_AIPerceptionComponent; }
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetHitRecovery() const { return m_HitRecovery; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsDead() const { return m_bIsDead; }
	FORCEINLINE int32 GetHitDirection() const { return m_HitDirection; }
	FORCEINLINE UShapeComponent* GetCollider(const FName& colliderName) { return m_Colliders[colliderName].Get(); }
	
	
	bool HasContainHitActor(const FName& attackName, AActor* hitActor)
	{
		return m_AttackInformations[attackName].checkedHitActors.Contains(hitActor);
	}
	
	void AddCheckedHitActor(const FName& attackName, AActor* hitActor)
	{
		m_AttackInformations[attackName].checkedHitActors.Add(hitActor,true);
	}

	void EmptyCheckedHitActor(const FName& attackName)
	{
		m_AttackInformations[attackName].checkedHitActors.Empty();
	}

	
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ExecOnHitEvent(ACharacterBase* instigator){};
	virtual void OnHPIsZero();
	virtual void Die() {};
	virtual void ExecEvent_EndedDeathMontage() {}; 
										  

protected:
	TMap<FName, FAttackInfo> m_AttackInformations;
	TMap<FName, TWeakObjectPtr<UShapeComponent>> m_Colliders;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStatComponent* m_StatComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UAIPerceptionComponent* m_AIPerceptionComponent;
	
	FTimerHandle m_OnHitTimerHandle;
	FTimerHandle m_DeathTimerHandle;
	FTimerHandle m_DeActivateTimerHandle;
	
	UPROPERTY(EditDefaultsOnly) 
	float m_WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float m_RunSpeed;

	UPROPERTY(EditDefaultsOnly)
	float m_HitRecovery;

	UPROPERTY(EditDefaultsOnly)
	float m_OnHitTimerTime;

	UPROPERTY(EditDefaultsOnly)
	float m_DeathTimerTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	float m_CurSpeed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsSuperArmor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsDead;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	int32 m_HitDirection;
	
	float m_DeathTimerTickTime;
	float m_DeathTimerRemainingTime;
	float m_DiffuseRatio;
};
