// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "MeleeMinion.generated.h"

enum class ENormalMinionStates : uint8;

UCLASS()
class HYOBINSPROJECT_API AMeleeMinion : public AMonster
{
	GENERATED_BODY()

public:
	AMeleeMinion();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	ENormalMinionStates GetState() { return m_CurState; }
	void SetState(ENormalMinionStates state) { m_CurState = state; }
	float GetNormalAttackRange() { return m_NormalAttackRange; }

	static int tagCount;

protected:
	virtual void BeginPlay() override;

private:
	void initComponents();
	void initCollisions();
	void initAttackInformations();

private:
	ENormalMinionStates m_CurState;
	float m_NormalAttackRange;
};
