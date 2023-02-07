// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "MeleeMinion.generated.h"

/**
 * 
 */
UCLASS()
class HYOBINSPROJECT_API AMeleeMinion : public AMonster
{
	GENERATED_BODY()

public:
	AMeleeMinion();

private:
	void initComponents();
	void loadMesh();
	void loadAnimInstance();
	void initSwordCollision();
	void initAttackInformations();
};
