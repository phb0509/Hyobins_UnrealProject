// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AIControllerBase.h"
#include "MeleeMinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AMeleeMinionAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AMeleeMinionAIController(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnPossess(APawn* pawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


protected:
	//UFUNCTION()
		virtual void UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus) override;


private:
	void initPerceptionSystem();


private:
	TWeakObjectPtr<class AMeleeMinion> m_Owner;
};
