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
	Idle 						UMETA(DisplayName = "Idle"),
	NormalAttack				UMETA(DisplayName = "NormalAttack"),
	NormalStrikeAttack			UMETA(DisplayName = "NormalStrikeAttack"),
	Dodge_NonTargeting 			UMETA(DisplayName = "Dodge_NonTargeting"),
	Dodge_Targeting 			UMETA(DisplayName = "Dodge_Targeting"),
	Parrying					UMETA(DisplayName = "Parrying"),
	ParryingAttack				UMETA(DisplayName = "ParryingAttack")
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
	Knockback		UMETA(DisplayName = "Knockback"),
	Groggy			UMETA(DisplayName = "Groggy"),
	Dead			UMETA(DisplayName = "Dead"),
	NormalAttack	UMETA(DisplayName = "NormalAttack")
};

UENUM(BlueprintType)
enum class ECrowdControlType : uint8
{
	None		UMETA(DisplayName = "None"),
	Knockback	UMETA(DisplayName = "Knockback"),
	Down		UMETA(DisplayName = "Down"),
	Groggy		UMETA(DisplayName = "Groggy"),
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
