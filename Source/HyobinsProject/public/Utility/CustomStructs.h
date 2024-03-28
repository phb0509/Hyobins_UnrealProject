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
struct FHPAttackInformationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHPAttackInformationData() :
	attackName(""),
	damage(-1.0f),
	bIsDot(false),
	bHasCrowdControl(false),
	crowdControlType("None"),
	crowdControlTime(-1.0f),
	bHasKnockBack(false),
	knockBackTime(-1.0f), knockBackDistance(-1.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName attackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasCrowdControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName crowdControlType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float crowdControlTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasKnockBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float knockBackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float knockBackDistance;
};


USTRUCT(Atomic, BlueprintType) // Atomic : �׻� �ϳ��� ������ ����ȭ. BlueprintType : �������Ʈ���� ���� �� ������ �ǹ�.
struct FAttackInfoStruct : public FDamageEvent
{
	GENERATED_USTRUCT_BODY() // �𸮾� ������Ʈ���� �˸��� ��ũ��

public:
	FName attackName;
	float damage;
	bool bIsDot; 
	bool bHasCrowdControl;
	ECrowdControlType crowdControlType; // CC�� ����
	float crowdControlTime; // CC�� ���ӽð�
	bool bHasKnockBack; 
	float knockBackTime; // �����ð� -> �ǰ�ü�� ��Ʈ��� ����ð��� ����
	float knockBackDistance; // �з����� ����
	FVector colliderLocation;
	TMap<AActor*, bool> checkHitActors;
};

