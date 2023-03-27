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


USTRUCT(Atomic, BlueprintType) // Atomic : 항상 하나의 단위로 직렬화. BlueprintType : 블루프린트에서 사용될 수 있음을 의미.
struct FAttackInfoStruct : public FDamageEvent
{
	GENERATED_USTRUCT_BODY() // 언리얼 오브젝트임을 알리는 매크로

public:
	
	FName attackName;
	float damage;
	bool bIsDot; 
	bool bHasCrowdControl;
	ECrowdControlType crowdControlType; // CC기 종류
	float crowdControlTime; // CC기 지속시간
	bool bHasKnockBack; 
	float knockBackTime; // 경직시간 -> 피격체의 힛트모션 재생시간과 대응
	float knockBackDistance; // 밀려지는 정도
};

