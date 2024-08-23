// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomStructs.generated.h"

enum class ECrowdControlType : uint8;

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
	airbornePower(0.0f),
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
	float airbornePower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool canDodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool canParrying;
};


USTRUCT(Atomic, BlueprintType) // Atomic : �׻� �ϳ��� ������ ����ȭ. BlueprintType : �������Ʈ���� ���� �� ������ �ǹ�.
struct FAttackInfo : public FDamageEvent
{
	GENERATED_USTRUCT_BODY() // �𸮾� ������Ʈ���� �˸��� ��ũ��

public:
	FName attackName;
	float damageRatio;
	bool bHasCrowdControl;
	ECrowdControlType crowdControlType; // CC�� ����
	float crowdControlTime; // CC�� ���ӽð�
	float knockBackDistance; // �з����� ����
	float airbornePower;
	FVector colliderLocation;
	bool canDodge;
	bool canParrying;
};

