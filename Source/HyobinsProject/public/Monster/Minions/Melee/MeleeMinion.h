// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "MeleeMinion.generated.h"

enum class ENormalMinionStates : uint8;

UCLASS()
class HYOBINSPROJECT_API AMeleeMinion final: public AMonster
{
	GENERATED_BODY()

public:
	AMeleeMinion();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void OnHitTimerEnded() override;

	void NormalAttack();

	// Get
	ENormalMinionStates GetState() { return m_CurState; }
	float GetNormalAttackRange() { return m_NormalAttackRange; }

	// Set
	void SetState(ENormalMinionStates state);

protected:
	virtual void BeginPlay() override;
	virtual void SetHitState() override;
	virtual void ExecHitEvent(ACharacterBase* instigator) override;
	

private:
	void initAssets();
	void updateState();
	void onNormalAttackMontageEnded();
	void Die();

	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	static int TagCount;

private:
	ENormalMinionStates m_CurState;

	TWeakObjectPtr<class UMeleeMinionAnim> m_AnimInstance;
	TWeakObjectPtr<class AMeleeMinionAIController> m_AIController;

};
