// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HyobinsProjectGameModeBase.generated.h"

class UEnvironmentSettings;


UCLASS()
class HYOBINSPROJECT_API AHyobinsProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHyobinsProjectGameModeBase();

	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void BeginPlay() override;


private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEnvironmentSettings> m_EnvironmentSettingsUI;
};
