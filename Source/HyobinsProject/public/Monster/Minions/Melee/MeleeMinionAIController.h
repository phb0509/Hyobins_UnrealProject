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
	AMeleeMinionAIController();
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
		void CheckIsTarget(AActor* actor, FAIStimulus const Stimulus);

	/*UFUNCTION()
		void UpdatePerception(const TArray<AActor*>& Actors);*/

private:
	void initPerceptionSystem();


public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName PlayerKey;
	static const FName InNormalAttackRangeKey;

private:
	TWeakObjectPtr<APawn> m_Owner;
};
