// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterManager.generated.h"

UCLASS()
class HYOBINSPROJECT_API AMonsterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void CreateMonster();


public:
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float m_MinTime;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float m_MaxTime;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> m_SpawnPoints;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AMonster> m_MonsterFactory;

private:
	FTimerHandle m_SpawnTimerHandle; // 월드에 하나있는 글로벌 타이머매니저에게 이 변수를 넘긴 후,
									 // 원하는 시간이 되면 알려달라고 요청.
									 // 이 때 타이머 매니저한테 알림을 받을 함수 혹은 델리게이트를 넘겨준다.

};
