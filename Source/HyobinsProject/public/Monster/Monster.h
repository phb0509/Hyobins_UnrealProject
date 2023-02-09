// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CustomStructs.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class HYOBINSPROJECT_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
protected:
	void loadMesh(FString path);
	void loadAnimInstance(FString path);

	

protected:
	float m_MaxHP;
	float m_CurHP;
	TMap<FString, FAttackInfoStruct> m_AttackInformations;

	float m_PatrolRange;
};
