// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/IsIntegral.h"
#include "Templates/IsEnumClass.h"
#include "EnumTypes.generated.h"


UENUM(BlueprintType)
enum class ECrowdControlStates : uint8
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
	Idle 								UMETA(DisplayName = "Idle"),
	NormalAttack_OnGround				UMETA(DisplayName = "NormalAttack_OnGround"),
	NormalStrikeAttack_OnGround			UMETA(DisplayName = "NormalStrikeAttack_OnGround"),
	DashAttack_OnGround					UMETA(DisplayName = "DashAttack_OnGround"),
	DashAttack_InAir					UMETA(DisplayName = "DashAttack_InAir"),
	UpperAttack_OnGround				UMETA(DisplayName = "UpperAttack_OnGround"),
	UpperAttack_GroundToAir				UMETA(DisplayName = "UpperAttack_GroundToAir"),
	NormalAttack_InAir					UMETA(DisplayName = "NormalAttack_InAir"),
	EarthStrike_FallingToGround			UMETA(DisplayName = "EarthStrike_FallingToGround"),
	EarthStrike_OnGround				UMETA(DisplayName = "EarthStrike_OnGround"),
	Charging_OnGround					UMETA(DisplayName = "Charging_OnGround"),
	StopCharging						UMETA(DisplayName = "StopCharging"),
	Charging_ComboDashAttack_OnGround	UMETA(DisplayName = "Charging_ComboDashAttack_OnGround"),
	Dodge_NonTargeting_OnGround 		UMETA(DisplayName = "Dodge_NonTargeting_OnGround"),
	Dodge_Targeting_OnGround			UMETA(DisplayName = "Dodge_Targeting_OnGround"),
	Guard_OnGround 						UMETA(DisplayName = "Guard_OnGround"),
	Parry_OnGround 						UMETA(DisplayName = "Parry_OnGround")
};


UENUM(BlueprintType)
enum class ESuperMinionFSMStates : uint8
{
	Patrol					UMETA(DisplayName = "Patrol"),
	Chase					UMETA(DisplayName = "Chase"),
	NormalAttack			UMETA(DisplayName = "NormalAttack")
};

UENUM(BlueprintType)
enum class ELichKingFSMStates : uint8
{
	Chase					UMETA(DisplayName = "Chase"),
	NormalAttack			UMETA(DisplayName = "NormalAttack"),
	CrashAttack				UMETA(DisplayName = "CrashAttack"),
	SoulSiphon				UMETA(DisplayName = "SoulSiphon"),
	Groggy					UMETA(DisplayName = "Groggy"),
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

template <typename T1, typename T2>
typename TEnableIf< (TIsEnumClass<T1>::Value || TIsIntegral<T1>::Value) &&
					(TIsEnumClass<T2>::Value || TIsIntegral<T2>::Value),bool>::Type
operator== (const T1 value1, const T2 value2)
{
	return static_cast<uint8>(value1) == static_cast<uint8>(value2);
}


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
