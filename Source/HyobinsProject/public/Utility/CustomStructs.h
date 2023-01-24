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


USTRUCT(Atomic, BlueprintType) // Atomic : 항상 하나의 단위로 직렬화. BlueprintType : 블루프린트에서 사용될 수 있음을 의미.
struct FAttackInfoStruct
{
	GENERATED_USTRUCT_BODY() // 언리얼 오브젝트임을 알리는 매크로

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // 포인터변수의 경우, 댕글링포인터 방지하기위해 UPROPERTY 꼭 붙이기
	float damage;
};
