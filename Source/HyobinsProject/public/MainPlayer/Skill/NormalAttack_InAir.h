// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer/Skill/Skill.h"
#include "NormalAttack_InAir.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UNormalAttack_InAir : public USkill
{
	GENERATED_BODY()

public:
	UNormalAttack_InAir();

	virtual void Initialize() override;
	virtual void Execute() override;

protected:
	virtual bool GetCanExecuteSkill() const override;
private:
	void linqNextNormalAttackInAirCombo(); // 섹션점프
	
	
private:
	int32 m_CurComboAttackSection;
	int32 m_MaxNormalAttackSection;

	UPROPERTY(EditAnywhere, Category = "MoveDistance")
	float m_MoveDistance;
};
