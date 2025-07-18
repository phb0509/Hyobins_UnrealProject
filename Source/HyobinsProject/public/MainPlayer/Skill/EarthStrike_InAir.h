// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "EarthStrike_InAir.generated.h"

class UParticleSystem;
class USoundWave;

UCLASS()
class HYOBINSPROJECT_API UEarthStrike_InAir : public USkill
{
	GENERATED_BODY()

public:
	UEarthStrike_InAir();

	virtual void Initialize() override;
	virtual void Execute() override;

	void ExecEvent_WhenOnGround();
	void attack();
	void playEffect();

protected:
	virtual bool CanExecuteSkill() const override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_FallingToGroundMontage;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_EarthStrikeMontage;

	FTimerHandle m_Timer;

	UPROPERTY(EditAnywhere, Category = "AttackRange")
	float m_AttackRangeRadius;
	
	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<UParticleSystem> m_Particle;
	
	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> m_CameraShake;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundWave> m_Sound;
};


