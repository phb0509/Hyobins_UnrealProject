// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer/Skill/Skill.h"
#include "DashAttack_InAir.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UDashAttack_InAir : public USkill
{
	GENERATED_BODY()
	
public:
	UDashAttack_InAir();

	virtual void Initialize() override;
	virtual void Execute() override;
	
protected:
	virtual bool GetCanExecuteSkill() const override;
	
private:
	UPROPERTY(EditAnywhere, Category = "MoveDistance")
	float m_MoveDistance;
};
