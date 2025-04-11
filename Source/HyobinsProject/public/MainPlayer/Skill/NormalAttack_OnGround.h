// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "NormalAttack_OnGround.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UNormalAttack_OnGround : public USkill
{
	GENERATED_BODY()
	
public:
	UNormalAttack_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;
	
	int32 GetCurComboAttackSection() const { return m_CurComboAttackSection; }

protected:
	virtual bool GetCanExecuteSkill() const override;
	
private:
	void linqNextNormalAttackOnGroundCombo();

private:
	int32 m_CurComboAttackSection;
	int32 m_MaxNormalAttackSection;

	UPROPERTY(EditAnywhere, Category = "MoveDistance")
	float m_MoveDistance;
};
