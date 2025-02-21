// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer/Skill/Skill.h"
#include "Dodge_OnGround.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HYOBINSPROJECT_API UDodge_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UDodge_OnGround();
	
	virtual void Initialize() override;
	virtual void Execute() override;

private:
	UPROPERTY(EditAnywhere, Category = "Dodge_OnGround")
	float m_MoveDistance;
};
