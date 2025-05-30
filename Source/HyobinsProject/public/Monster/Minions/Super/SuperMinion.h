// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SuperMinion.generated.h"

class UMotionWarpingComponent;

UCLASS()
class HYOBINSPROJECT_API ASuperMinion final: public AMonster
{
	GENERATED_BODY()

public:
	ASuperMinion();
	
	virtual void BeginPlay() override;
	
	
protected:
	virtual void Activate() override;

private:
	void initAssets();
	void printLog();
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_LeftSwordCollider;
		
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_RightSwordCollider;
	
	static int32 TagCount;
	static const FName LeftSwordColliderName;
	static const FName RightSwordColliderName;
};
