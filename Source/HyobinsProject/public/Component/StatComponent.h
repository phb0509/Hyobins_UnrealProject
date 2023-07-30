// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

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
	void SetDamage(float damage);
	FOnHPIsZeroDelegate OnHPIsZero;

private:
	UPROPERTY(EditDefaultsOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float m_MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float m_CurHP;

};
