// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AIControllerBase.h"
#include "SuperMinionAIController.generated.h"

class ASuperMinion;

UCLASS()
class HYOBINSPROJECT_API ASuperMinionAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ASuperMinionAIController(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable)
	void UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus);


private:
	void initAssets();

private:
	TWeakObjectPtr<ASuperMinion> m_Owner;
	
};
