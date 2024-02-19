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
	Attack	UMETA(DisplayName = "Attack"),
	Hit		UMETA(DisplayName = "Hit"),
	Die     UMETA(DisplayName = "Die")
};

UENUM(BlueprintType)
enum class ENormalMinionStates : uint8
{
	Patrol			UMETA(DisplayName = "Patrol"),
	Chase			UMETA(DisplayName = "Chase"),
	NormalAttack	UMETA(DisplayName = "NormalAttack"),
	Hit				UMETA(DisplayName = "Hit"),
	Die				UMETA(DisplayName = "Die"),
	End				UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EMonsterCommonStates : uint8
{
	Patrol			UMETA(DisplayName = "Patrol"),
	Hit				UMETA(DisplayName = "Hit"),
	Die				UMETA(DisplayName = "Die"),
	End				UMETA(DisplayName = "End")
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
	virtual void BeginPlay() override;

public:	

public:
	UPROPERTY(EditAnywhere)
		ENormalMinionStates m_NormalMinionStates;

};
