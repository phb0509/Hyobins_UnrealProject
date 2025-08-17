// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "Execution_OnGround.generated.h"

class ACharacterBase;

UCLASS()
class HYOBINSPROJECT_API UExecution_OnGround : public USkill
{
	GENERATED_BODY()

public:
    UExecution_OnGround();
    
	virtual void Initialize() override;
	virtual void Execute() override;

    
protected:
	virtual bool CanExecuteSkill() const override;

private:
	ACharacterBase* findTarget();
	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_ExecutionMontage;

	TWeakObjectPtr<ACameraActor> m_ExecutionCamera;
	
	UPROPERTY(EditAnywhere, Category = "ExecutionCameraSettings")
	float m_ExecutionCameraBlendTime;

	UPROPERTY(EditAnywhere, Category = "ExecutionCameraSettings")
	float m_ExecutionCameraDistance;
	
	UPROPERTY(EditAnywhere, Category = "ExecutionCameraSettings")
	float m_ExecutionCameraFOV;
};
