// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HYOBINSPROJECT_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();
	
	virtual void BeginPlay() override;
	
	FORCEINLINE void InitHP() { m_CurHP = m_MaxHP; }
	FORCEINLINE void ChangeMaxHP(const float hp) { m_MaxHP = hp; }
	
	FORCEINLINE void AddMoveSpeed(const float additionalMoveSpeed)
	{
		m_CurAdditionalMoveSpeed += additionalMoveSpeed;
		m_CurMoveSpeed = 1.0f + m_CurAdditionalMoveSpeed / 100.0f;
	}
	
	FORCEINLINE void AddAttackSpeed(const float additionalAttackSpeed)
	{
		m_CurAdditionalAttackSpeed += additionalAttackSpeed;
		m_CurAttackSpeed = 1.0f + m_CurAdditionalAttackSpeed / 100.0f;
	}
	
	// Get
	FORCEINLINE float GetDefaultDamage() const { return m_DefaultDamage; }
	FORCEINLINE int32 GetCriticalAttackChance() const { return m_CriticalAttackChance; }
	FORCEINLINE float GetHPRatio() const { return m_CurHP < KINDA_SMALL_NUMBER ? 0.0f : (m_CurHP / m_MaxHP); }
	FORCEINLINE float GetHitRecovery() const { return m_HitRecovery; }
	FORCEINLINE float GetMoveSpeed() const { return m_CurMoveSpeed; }
	FORCEINLINE float GetAttackSpeed() const { return m_CurAttackSpeed; }
	
	// Set
	void OnDamage(const float damage);
	void SetHP(const float hp);
	
	FOnHPIsChangedDelegate OnHPIsChanged;
	FOnHPIsZeroDelegate OnHPIsZero;
	
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_DefaultDamage;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	int32 m_CriticalAttackChance;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_Defense;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float m_MaxHP;

	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	float m_CurHP;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	float m_HitRecovery;
	
	float m_CurMoveSpeed;
	float m_CurAdditionalMoveSpeed;
	float m_CurAttackSpeed;
	float m_CurAdditionalAttackSpeed;
};
