// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "Guard_OnGround.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API UGuard_OnGround : public USkill
{
	GENERATED_BODY()
	
public:
	UGuard_OnGround();

	virtual void Initialize() override;
	virtual void Execute() override;

protected:
	virtual bool CanExecuteSkill() const override;

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_GuardMontage;
	
	FTimerHandle m_ParryingTimer;

	UPROPERTY(EditAnywhere)
	float m_ParryingTime;
};
