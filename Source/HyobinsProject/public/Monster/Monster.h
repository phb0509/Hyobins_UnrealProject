// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "Monster.generated.h"

UCLASS()
class HYOBINSPROJECT_API AMonster : public ACharacterBase
{
	GENERATED_BODY()

public:
	AMonster();

	float GetPatrolRange() { return m_PatrolRange; }
	float GetNormalAttackRange() { return m_NormalAttackRange; }

protected:
	TMap<FString, FAttackInfoStruct> m_AttackInformations;
	float m_PatrolRange;
	float m_NormalAttackRange;
	
	
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName EnemyKey;
	static const FName CurStateKey;
	//static UEnum* m_StatesEnum;

};
