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
	FTimerHandle m_SpawnTimerHandle; // ���忡 �ϳ��ִ� �۷ι� Ÿ�̸ӸŴ������� �� ������ �ѱ� ��,
									 // ���ϴ� �ð��� �Ǹ� �˷��޶�� ��û.
									 // �� �� Ÿ�̸� �Ŵ������� �˸��� ���� �Լ� Ȥ�� ��������Ʈ�� �Ѱ��ش�.

};
