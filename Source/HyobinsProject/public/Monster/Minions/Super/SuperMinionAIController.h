// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase/AIControllerBase.h"
#include "SuperMinionAIController.generated.h"

class ASuperMinion;
class UAIPerceptionComponent;

UCLASS()
class HYOBINSPROJECT_API ASuperMinionAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ASuperMinionAIController(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;
	
	UFUNCTION(BlueprintCallable)
	void UpdatePerceptedTargetActor(AActor* actor, FAIStimulus const Stimulus);
	
	// Get
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


private:
	void initAssets();

private:
	TWeakObjectPtr<ASuperMinion> m_Owner;
	
};
