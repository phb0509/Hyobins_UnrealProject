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

	// Get
	FORCEINLINE float GetHPRatio() const;

	// Set
	void SetDamage(const float damage);
	void SetHP(const float hp);

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPIsChangedDelegate OnHPIsChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float m_MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float m_CurHP;
};
