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
	virtual void InitializeComponent() override;

public:
	void InitHP() { m_CurHP = m_MaxHP; }
	void ChangeMaxHP(float hp) { m_MaxHP = hp; }

	// Get
	float GetHPRatio();

	// Set
	void SetDamage(float damage);
	void SetHP(float hp);

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPIsChangedDelegate OnHPIsChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float m_MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float m_CurHP;

};
