// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.generated.h"

/**
 * 
 */
class HYOBINSPROJECT_API EnumTypes
{
public:
	EnumTypes();
	~EnumTypes();
};

UENUM(BlueprintType)
enum class EMainPlayerStates : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Walk	UMETA(DisplayName = "Walk"),
	Run		UMETA(DisplayName = "Run"),
	Attack	UMETA(DisplayName = "Attack"),
	Hit		UMETA(DisplayName = "Hit"),
	Die     UMETA(DisplayName = "Die")
};

UENUM(BlueprintType)
enum class EMinionStates : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Walk	UMETA(DisplayName = "Walk"),
	Run		UMETA(DisplayName = "Run"),
	Attack	UMETA(DisplayName = "Attack"),
	Hit		UMETA(DisplayName = "Hit"),
	Die     UMETA(DisplayName = "Die")
};