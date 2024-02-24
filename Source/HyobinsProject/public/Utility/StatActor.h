// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Component/StatComponent.h"
#include "StatActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatActor : public UInterface
{
	GENERATED_BODY()
};


class HYOBINSPROJECT_API IStatActor // 추상클래스처럼 사용.
{
	GENERATED_BODY()

public:
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }

protected:
	UStatComponent* m_StatComponent;
};