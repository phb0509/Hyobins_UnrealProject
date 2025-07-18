// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStatIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedStatDelegate, float changeAmount);

class ACharacterBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HYOBINSPROJECT_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();
	
	virtual void BeginPlay() override;
	
	FORCEINLINE void InitHP() { m_CurHP = m_MaxHP; }
	FORCEINLINE void SetMaxHP(const float maxHP) { m_MaxHP = maxHP; }

	FORCEINLINE void AddDefense(const float additionalDefense)
	{
		m_Defense += additionalDefense;
	}
	
	void AddMoveSpeed(const float additionalMoveSpeed)
	{
		m_CurAdditionalMoveSpeed += additionalMoveSpeed;
		m_CurMoveSpeed = 1.0f + m_CurAdditionalMoveSpeed / 100.0f;
	}
	
	void AddAttackSpeed(const float additionalAttackSpeed)
	{
		m_CurAdditionalAttackSpeed += additionalAttackSpeed;
		m_CurAttackSpeed = 1.0f + m_CurAdditionalAttackSpeed / 100.0f;
	}

	void RecoveryHP();
	void RecoveryStamina();
	
	FORCEINLINE bool HasEnoughStamina(const float cost) const { return cost <= m_CurStamina; } 
	
	// Get
	FORCEINLINE float GetDefense() const { return m_Defense; }
	FORCEINLINE float GetDefaultDamage() const { return m_DefaultDamage; }
	FORCEINLINE int32 GetCriticalAttackChance() const { return m_CriticalAttackChance; }

	FORCEINLINE float GetCurHP() const { return m_CurHP; }
	FORCEINLINE float GetMaxHP() const { return m_MaxHP; }
	FORCEINLINE float GetHPRatio() const { return m_CurHP < KINDA_SMALL_NUMBER ? 0.0f : (m_CurHP / m_MaxHP); }
	FORCEINLINE bool CanRecoveryHP() const { return m_bCanRecoveryHP; }

	FORCEINLINE float GetCurStamina() const { return m_CurStamina; }
	FORCEINLINE float GetMaxStamina() const { return m_MaxStamina; }
	FORCEINLINE float GetStaminaRatio() const { return m_CurStamina < KINDA_SMALL_NUMBER ? 0.0f : (m_CurStamina / m_MaxStamina); }
	FORCEINLINE bool CanRecoveryStamina() const { return m_bCanRecoveryStamina; }
	
	FORCEINLINE float GetHitRecovery() const { return m_HitRecovery; }
	FORCEINLINE float GetMoveSpeed() const { return m_CurMoveSpeed; }
	FORCEINLINE float GetAttackSpeed() const { return m_CurAttackSpeed; }

	FORCEINLINE void SetCanRecoveryHP(bool bCanRecoveryHP) { m_bCanRecoveryHP = bCanRecoveryHP; }
	FORCEINLINE void SetCanRecoveryStamina(bool bCanRecoveryStamina) { m_bCanRecoveryStamina = bCanRecoveryStamina; }
	
	// Set
	FORCEINLINE void SetDefense(float defense) { m_Defense = defense; }
	FORCEINLINE void SetAdditionalDefenseFromGuard(float additionalDefenseFromGuard) { m_AdditionalDefenseFromGuard = additionalDefenseFromGuard; }

	FORCEINLINE void AddAdditionalDefenseFromGuard() { m_Defense += m_AdditionalDefenseFromGuard; }
	FORCEINLINE void RemoveAdditionalDefenseFromGuard() { m_Defense -= m_AdditionalDefenseFromGuard; }
	
	void OnDamageHP(const float damage);
	void SetHPPercent(const float hp);

	void OnDamageStamina(const float damage);
	void SetStaminaPercent(const float stamina);

	FORCEINLINE float CalculateFinalCrowdControlTime(const float crowdControlTime) const { return m_HitRecovery * crowdControlTime; }
	FORCEINLINE float CalculateFinalDamage(const float damage) const { return damage - m_Defense > 0.0f ? damage - m_Defense : 0.0f; }
	
public:
	FOnChangedStatDelegate OnChangedHP;
	FOnStatIsZeroDelegate OnHPIsZero;  

	FOnChangedStatDelegate OnChangedStamina;
	FOnStatIsZeroDelegate OnStaminaIsZero;

	
private:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_DefaultDamage;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	int32 m_CriticalAttackChance;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_Defense;

	float m_AdditionalDefenseFromGuard;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_MaxHP;

	UPROPERTY(VisibleInstanceOnly, Transient, Meta = (AllowPrivateAccess = true))
	float m_CurHP;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_MaxStamina;

	UPROPERTY(VisibleInstanceOnly, Transient, Meta = (AllowPrivateAccess = true))
	float m_CurStamina;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	float m_HitRecovery;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_HPRecoveryPerSecond;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_StaminaRecoveryPerSecond;
	
	float m_CurMoveSpeed;
	float m_CurAdditionalMoveSpeed;
	float m_CurAttackSpeed;
	float m_CurAdditionalAttackSpeed;

	FTimerHandle m_HPRecoveryTimer;
	FTimerHandle m_StaminaRecoveryTimer;

	bool m_bCanRecoveryHP;
	bool m_bCanRecoveryStamina;
};
