// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AIControllerBase.h"
#include "SuperMinionAIController.generated.h"


UCLASS()
class HYOBINSPROJECT_API ASuperMinionAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ASuperMinionAIController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
		void CheckIsTarget(AActor* actor, FAIStimulus const Stimulus);


private:
	void initPerceptionSystem();


private:
	TWeakObjectPtr<class ASuperMinion> m_Owner;
};
