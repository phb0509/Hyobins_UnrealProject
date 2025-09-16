// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utility/CustomStructs.h"
#include "DataManager.generated.h"


UCLASS()
class HYOBINSPROJECT_API UDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void LoadAttackInformation(TSubclassOf<AActor> classType, const FString& assetPath);
	void DeleteAttackInformation(TSubclassOf<AActor> classType);
	void InitHitActors(TSubclassOf<AActor> classType, OUT TMap<FName, TSet<TWeakObjectPtr<AActor>>>& hitActors);
	
	const FAttackInformation* GetAttackInformation(TSubclassOf<AActor> classType, const FName& attackName) const;
	
private:
	TMap<TSubclassOf<AActor>, TMap<FName,FAttackInformation>> m_AttackInformations;
};
