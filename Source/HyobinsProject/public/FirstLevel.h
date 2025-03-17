// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBase.h"
#include "FirstLevel.generated.h"

class AActorPool;
class AMonster;
class IPoolableActor;
struct FStreamableHandle;

UCLASS()
class HYOBINSPROJECT_API AFirstLevel : public ALevelBase
{
	GENERATED_BODY()
	
public:
	AFirstLevel();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void CreateMinions() const;
	
	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void SpawnBoss();

	UFUNCTION(BlueprintCallable, Category = "FirstLevel EventFunction")
	void SpawnMinion();

private:
	FTimerHandle m_SpawnTimerHandle_MeleeMinion;
	float m_SpawnTimerTime_MeleeMinion;

	TWeakObjectPtr<AActorPool> m_ActorPool;
	
	UPROPERTY(EditAnywhere, Category = "Spawn Monster")
	TSubclassOf<AActor> m_MonsterClass;
	
	UPROPERTY(EditAnywhere, Category = "Spawn Monster")
	TSubclassOf<AActor> m_LichKingClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Monster")
	float m_SpawnCount;
};
