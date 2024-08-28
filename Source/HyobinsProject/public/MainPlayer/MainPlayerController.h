// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "MainPlayerController.generated.h"


class UEnvironmentSettings;

UCLASS()
class HYOBINSPROJECT_API AMainPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMainPlayerController();
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	void ChangeEnvironmentSettingsState();

	

private:
	FGenericTeamId m_TeamID;

	FInputModeGameOnly m_GameInputMode;
    FInputModeUIOnly   m_UIInputMode;
	
	bool m_bIsOpenedEnvironmentSettings;
};
