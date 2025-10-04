// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase/AIControllerBase.h"
#include "LichKingAIController.generated.h"

class ALichKing;

UCLASS()
class HYOBINSPROJECT_API ALichKingAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ALichKingAIController(const FObjectInitializer& ObjectInitializer);
	
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
	TWeakObjectPtr<ALichKing> m_Owner;
};
