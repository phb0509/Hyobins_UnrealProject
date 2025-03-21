// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorPoolManager.generated.h"

class AActorPool;

UCLASS()
class HYOBINSPROJECT_API UActorPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	AActorPool* GetActorPool();


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActorPool> m_ActorPool;
	
};
