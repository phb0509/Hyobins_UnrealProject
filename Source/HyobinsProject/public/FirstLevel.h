// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBase.h"
#include "FirstLevel.generated.h"

class AActorPool;

UCLASS()
class HYOBINSPROJECT_API AFirstLevel : public ALevelBase
{
	GENERATED_BODY()
	
public:
	AFirstLevel();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void Create();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void Spawn();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void ClearActorPool();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void HideHPBar();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void ShowHPBar();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void ClearAllWidget();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void ClearHPBarWidget();


private:
	FTimerHandle m_SpawnTimerHandle_MeleeMinion;
	float m_SpawnTimerTime_MeleeMinion;

	TWeakObjectPtr<AActorPool> m_ActorPool;
};
