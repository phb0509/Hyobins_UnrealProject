// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer/Skill/Skill.h"
#include "Charging_OnGround.generated.h"

class ACharacterBase;

DECLARE_DELEGATE_TwoParams(FOnChargingDelegate, ACharacterBase*, float);
DECLARE_DELEGATE_OneParam(FOnStopChargingDelegate, ACharacterBase*);

UCLASS(BlueprintType)
class HYOBINSPROJECT_API UCharging_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UCharging_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;


public:
	FOnChargingDelegate OnCharging;
	FOnStopChargingDelegate OnStopCharging;
	
private:
	UPROPERTY(EditAnywhere, Category = "Duration")
	float m_ChargingDuration;

	FTimerHandle m_ChargingTimer;
};
