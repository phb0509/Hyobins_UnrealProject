// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomStructs.generated.h"


class HYOBINSPROJECT_API CustomStructs
{
public:
	CustomStructs();
	~CustomStructs();
};


USTRUCT(Atomic, BlueprintType) // Atomic : �׻� �ϳ��� ������ ����ȭ. BlueprintType : �������Ʈ���� ���� �� ������ �ǹ�.
struct FAttackInfoStruct
{
	GENERATED_USTRUCT_BODY() // �𸮾� ������Ʈ���� �˸��� ��ũ��

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // �����ͺ����� ���, ��۸������� �����ϱ����� UPROPERTY �� ���̱�
	float damage;
};
