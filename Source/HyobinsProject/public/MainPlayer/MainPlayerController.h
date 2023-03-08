// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AMainPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMainPlayerController();
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual FGenericTeamId GetGenericTeamId() const override; 


private:
	FGenericTeamId m_TeamID;
	
};
