// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBase.h"
#include "FirstLevel.generated.h"

class AActorPool;
class AMonster;

UCLASS()
class HYOBINSPROJECT_API AFirstLevel : public ALevelBase
{
	GENERATED_BODY()
	
public:
	AFirstLevel();

	virtual void BeginPlay() override;
	void Create();

private:
	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void Spawn();
	

private:
	FTimerHandle m_SpawnTimerHandle_MeleeMinion;
	float m_SpawnTimerTime_MeleeMinion;

	TWeakObjectPtr<AActorPool> m_ActorPool;


	UPROPERTY(EditAnywhere, Category = "Spawn Monster")
	TSubclassOf<AActor> m_SuperMinionClass;
};
