// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.generated.h"


UENUM(BlueprintType)
enum class EMainPlayerStates : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Walk	UMETA(DisplayName = "Walk"),
	Run		UMETA(DisplayName = "Run"),
	Hit		UMETA(DisplayName = "Hit"),
	Dead    UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EMainPlayerSkillStates : uint8
{
	None 					UMETA(DisplayName = "None"),
	NormalAttack_OnGround	UMETA(DisplayName = "NormalAttack_OnGround"),            
	UpperAttack_OnGround	UMETA(DisplayName = "UpperAttack_OnGround"),
	CombatDodge_OnGround 	UMETA(DisplayName = "CombatDodge_OnGround"),
	Parrying_OnGround		UMETA(DisplayName = "Parrying_OnGround"),
};


UENUM(BlueprintType)
enum class EMonsterCommonStates : uint8 // 몬스터들 공통상태. 하기의 몬스터관련 enum class들은 반드시 이 commonState들을 제일먼저 순서맞게 명시해야한다.
{
	Patrol			UMETA(DisplayName = "Patrol"),
	Chase			UMETA(DisplayName = "Chase"),
	Hit				UMETA(DisplayName = "Hit"),
	Dead			UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ENormalMinionStates : uint8
{
	Patrol			UMETA(DisplayName = "Patrol"),
	Chase			UMETA(DisplayName = "Chase"),
	Hit				UMETA(DisplayName = "Hit"),
	Dead			UMETA(DisplayName = "Dead"),
	NormalAttack	UMETA(DisplayName = "NormalAttack")
};

UENUM(BlueprintType)
enum class ECrowdControlType : uint8
{
	None    UMETA(DisplayName = "None"),
	Slow	UMETA(DisplayName = "Slow"),
	Bind	UMETA(DisplayName = "Bind"),
	Stun	UMETA(DisplayName = "Stun"),
	Airborne UMETA(DisplayName = "Airborne")
};

UCLASS()
class HYOBINSPROJECT_API AEnumTypes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnumTypes();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
};
