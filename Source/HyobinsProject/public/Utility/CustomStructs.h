// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/EnumTypes.h"
#include "CustomStructs.generated.h"


class HYOBINSPROJECT_API CustomStructs
{
public:
	CustomStructs();
	~CustomStructs();
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
};

