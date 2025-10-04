// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Utility/EnumTypes.h"
#include "CustomStructs.generated.h"

enum class ECrowdControlType : uint8;

class HYOBINSPROJECT_API CustomStructs
{
public:
	CustomStructs();
	~CustomStructs();
};

USTRUCT(Atomic)
struct FAttackInformationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAttackInformationData() :
	attackName(""),
	damageRatio(1.0f),
	bHasCrowdControl(false),
	crowdControlType(ECrowdControlType::None),
	crowdControlTime(0.0f),
	knockBackDistance(0.0f),
	airbornePower(0.0f),
	staminaDamage(10.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName attackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float damageRatio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasCrowdControl;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ECrowdControlType crowdControlType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float crowdControlTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float knockBackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float airbornePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float staminaDamage;
};


USTRUCT(Atomic) 
struct FAttackInformation
{
	GENERATED_USTRUCT_BODY() 

public:
	FName attackName;
	float damageRatio;
	ECrowdControlType crowdControlType; // CC기 종류
	float crowdControlTime; // CC기 지속시간
	float knockBackDistance; // 밀려지는 정도
	float airbornePower;
	float staminaDamage;
};

USTRUCT(Atomic) 
struct FHitInformation
{
	GENERATED_USTRUCT_BODY() 

public:
	FName attackName;
	TWeakObjectPtr<AActor> instigator;
	TWeakObjectPtr<AActor> hitActor;
	float finalDamage;
	bool bIsCriticalAttack;
	int32 hitDirection;
	ECrowdControlType crowdControlType; // CC기 종류
	float crowdControlTime; // CC기 지속시간
	float knockBackDistance; // 밀려지는 정도
	float airbornePower;
	float staminaDamage;
};

