// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/EnumTypes.h"
#include "Engine/DataTable.h"
#include "CustomStructs.generated.h"


class HYOBINSPROJECT_API CustomStructs
{
public:
	CustomStructs();
	~CustomStructs();
};

USTRUCT(BlueprintType)
struct FAttackInformationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAttackInformationData() :
	attackName(""),
	damageRatio(1.0f),
	bHasCrowdControl(false),
	crowdControlType("None"),
	crowdControlTime(0.0f),
	knockBackDistance(0.0f),
	canDodge(false),
	canParrying(false)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName attackName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float damageRatio;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool bHasCrowdControl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName crowdControlType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float crowdControlTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float knockBackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool canDodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool canParrying;
};


USTRUCT(Atomic, BlueprintType) // Atomic : 항상 하나의 단위로 직렬화. BlueprintType : 블루프린트에서 사용될 수 있음을 의미.
struct FAttackInfo : public FDamageEvent
{
	GENERATED_USTRUCT_BODY() // 언리얼 오브젝트임을 알리는 매크로

public:
	FName attackName;
	float damageRatio;
	bool bHasCrowdControl;
	ECrowdControlType crowdControlType; // CC기 종류
	float crowdControlTime; // CC기 지속시간
	float knockBackDistance; // 밀려지는 정도
	FVector colliderLocation;
	bool canDodge;
	bool canParrying;
	//TMap<TWeakObjectPtr<AActor>, bool> checkedHitActors;
};

