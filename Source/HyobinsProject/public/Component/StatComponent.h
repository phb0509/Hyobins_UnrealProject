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

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void InitHP() { m_CurHP = m_MaxHP; }
	FORCEINLINE void ChangeMaxHP(const float hp) { m_MaxHP = hp; }
	void AddMoveSpeed(const float additionalMoveSpeed);
	void AddAttackSpeed(const float additionalAttackSpeed);
	
	// Get
	FORCEINLINE float GetHPRatio() const { return m_CurHP < KINDA_SMALL_NUMBER ? 0.0f : (m_CurHP / m_MaxHP); }
	FORCEINLINE float GetMoveSpeed() const { return m_CurMoveSpeed; }
	FORCEINLINE float GetAttackSpeed() const { return m_CurAttackSpeed; }
	
	// Set
	void SetDamage(const float damage);
	void SetHP(const float hp);
	
	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPIsChangedDelegate OnHPIsChanged;

private:
	UPROPERTY(EditAnywhere, Category = StatComponent, Meta = (AllowPrivateAccess = true))
	float m_MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = StatComponent, Meta = (AllowPrivateAccess = true))
	float m_CurHP;
	float m_CurMoveSpeed;
	float m_CurAdditionalMoveSpeed;
	float m_CurAttackSpeed;
	float m_CurAdditionalAttackSpeed;
};
