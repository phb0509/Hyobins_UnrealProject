// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/CustomStructs.h"
#include "Kismet/GameplayStatics.h"
#include "HPGameInstance.h"
#include "CharacterBase.generated.h"

UCLASS()
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	
protected:
	void LoadMesh(FString assetPath);
	void LoadAnimInstance(FString assetPath);


protected:
	float m_MaxHP;
	float m_CurHP;
	FName m_Name;

	TMap<FName, FAttackInfoStruct> m_AttackInformations;
	
};
