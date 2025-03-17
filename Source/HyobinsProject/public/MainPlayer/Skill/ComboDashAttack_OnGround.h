// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer/Skill/Skill.h"
#include "ComboDashAttack_OnGround.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HYOBINSPROJECT_API UComboDashAttack_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UComboDashAttack_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;

	
};
