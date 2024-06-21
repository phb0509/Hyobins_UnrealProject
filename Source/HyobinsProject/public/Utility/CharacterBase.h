// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UStatComponent;
class AAIControllerBase;

enum class EMonsterCommonStates : uint8;

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void PossessedBy(AController* newController) override;
	virtual void OnCalledTimer_OnHit() {};
	virtual void OnCalledNotify_EndedDeath(); 
	
	// Get
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetHitRecovery() const { return m_HitRecovery; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsDead() const { return m_bIsDead; }
	FORCEINLINE int32 GetHitDirection() const { return m_HitDirection; }

	
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ExecOnHitEvent(ACharacterBase* instigator){};
	virtual void OnHPIsZero();
	virtual void Die() {};
	virtual void ExecEvent_EndedDeathMontage() {}; 
										  



protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = StatComponent)
	UStatComponent* m_StatComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Colliders, Meta = (AllowProtectedAccess = true))
	TArray<UShapeComponent*> m_HitColliders;

	TWeakObjectPtr<AAIControllerBase> m_AIControllerBase;
	
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
