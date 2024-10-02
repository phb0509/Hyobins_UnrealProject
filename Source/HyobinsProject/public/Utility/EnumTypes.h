// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.generated.h"


UENUM(BlueprintType)
enum class ECrowdControlState : uint8
{
	None					UMETA(DisplayName = "None"),
	KnockbackOnStanding		UMETA(DisplayName = "KnockbackOnStanding"),
	KnockbackInAir			UMETA(DisplayName = "KnockbackInAir"),
	Down					UMETA(DisplayName = "Down"),
	Groggy					UMETA(DisplayName = "Groggy"),
	Dead					UMETA(DisplayName = "Dead")
};

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
	KnockbackOnStanding				UMETA(DisplayName = "Knockback"),
	Groggy							UMETA(DisplayName = "Groggy"),
	Down							UMETA(DisplayName = "Down"),
	Idle 							UMETA(DisplayName = "Idle"),
	NormalAttack_OnGround			UMETA(DisplayName = "NormalAttack_OnGround"),
	NormalStrikeAttack_OnGround		UMETA(DisplayName = "NormalStrikeAttack_OnGround"),
	DashAttack_OnGround				UMETA(DisplayName = "DashAttack_OnGround"),
	DashAttack_InAir				UMETA(DisplayName = "DashAttack_InAir"),
	UpperAttack_OnGround			UMETA(DisplayName = "UpperAttack_OnGround"),
	UpperAttack_GroundToAir			UMETA(DisplayName = "UpperAttack_GroundToAir"),
	NormalAttack_InAir				UMETA(DisplayName = "NormalAttack_InAir"),
	EarthStrike_FallingToGround		UMETA(DisplayName = "EarthStrike_FallingToGround"),
	EarthStrike_OnGround			UMETA(DisplayName = "EarthStrike_OnGround"),
	Dodge_OnGround 					UMETA(DisplayName = "Dodge_OnGround"),
	Dodge_Targeting 				UMETA(DisplayName = "Dodge_Targeting"),
};


UENUM(BlueprintType)
enum class ESuperMinionFSMStates : uint8
{
	KnockbackOnStanding		UMETA(DisplayName = "KnockbackOnStanding"),
	KnockbackInAir			UMETA(DisplayName = "KnockbackInAir"),
	Down					UMETA(DisplayName = "Down"),
	Groggy					UMETA(DisplayName = "Groggy"),
	Patrol					UMETA(DisplayName = "Patrol"),
	Chase					UMETA(DisplayName = "Chase"),
	Dead					UMETA(DisplayName = "Dead"),
	NormalAttack			UMETA(DisplayName = "NormalAttack")
};

UENUM(BlueprintType)
enum class ECrowdControlType : uint8
{
	None		UMETA(DisplayName = "None"),
	Knockback	UMETA(DisplayName = "Knockback"),
	Airborne	UMETA(DisplayName = "Airborne"),
	Down		UMETA(DisplayName = "Down"),
	Groggy		UMETA(DisplayName = "Groggy")
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
