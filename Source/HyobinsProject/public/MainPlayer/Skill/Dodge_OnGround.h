// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "Dodge_OnGround.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UDodge_OnGround : public USkill
{
	GENERATED_BODY()

public:
	UDodge_OnGround();
	
	virtual void Initialize() override;
	virtual void Execute() override;
	
protected:
	virtual bool CanExecuteSkill() const override;


	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_TargetingDodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_NonTargetingDodgeMontage;

	UPROPERTY(EditAnywhere)
	float m_MoveDistance;

	UPROPERTY(EditAnywhere)
	float m_InvincibleTime;
	
	FTimerHandle m_InvincibilityTimer;
	
};
