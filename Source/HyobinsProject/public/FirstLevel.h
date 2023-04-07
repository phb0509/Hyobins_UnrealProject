// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBase.h"
#include "FirstLevel.generated.h"


UCLASS()
class HYOBINSPROJECT_API AFirstLevel : public ALevelBase
{
	GENERATED_BODY()
	
public:
	AFirstLevel();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	void spawn();

	//template<class T>
	//void TestFunction(T* classType);
	


private:
	FTimerHandle m_SpawnTimerHandle_MeleeMinion;
	float m_SpawnTimerTime_MeleeMinion;
	
};
