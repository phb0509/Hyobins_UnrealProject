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

	FString GetName() { return m_Name; }
	float GetCurSpeed() { return m_CurSpeed; }
	bool GetIsIdle() { return m_bIsIdle; }
	bool GetIsWalking() { return m_bIsWalking; }
	bool GetIsInAir() { return m_bIsInAir; }


protected:
	void LoadMesh(FString assetPath);
	void LoadAnimInstance(FString assetPath);


protected:
	TMap<FName, FAttackInfoStruct> m_AttackInformations;

	float m_MaxHP;
	float m_CurHP;
	FString m_Name;

	float m_CurSpeed;
	float m_WalkSpeed;
	bool m_bIsIdle;
	bool m_bIsWalking;
	bool m_bIsInAir;
};
