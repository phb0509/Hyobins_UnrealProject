// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer/Skill/Skill.h"
#include "DashAttack_OnGround.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HYOBINSPROJECT_API UDashAttack_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UDashAttack_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;

private:
	UPROPERTY(EditAnywhere, Category = "MoveDistance")
	float m_MoveDistance;
};
