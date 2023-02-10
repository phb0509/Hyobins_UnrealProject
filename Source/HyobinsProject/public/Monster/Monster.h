// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CustomStructs.h"
#include "Utility/CharacterBase.h"
#include "Monster.generated.h"

UCLASS()
class HYOBINSPROJECT_API AMonster : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	

protected:
	TMap<FString, FAttackInfoStruct> m_AttackInformations;

	float m_PatrolRange;
};
