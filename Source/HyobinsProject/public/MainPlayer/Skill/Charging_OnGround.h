// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "Charging_OnGround.generated.h"


UCLASS()
class HYOBINSPROJECT_API UCharging_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UCharging_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;

protected:
	virtual bool CanExecuteSkill() const override;

private:
	UPROPERTY(EditAnywhere, Category = "Duration")
	float m_ChargingDuration;
	
};
