// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "ChargingCancel_OnGround.generated.h"


UCLASS()
class HYOBINSPROJECT_API UChargingCancel_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UChargingCancel_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;

protected:
	virtual bool CanExecuteSkill() const override;
	
};
