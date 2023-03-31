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
	virtual void OnHitTimerEnded() override;

	void NormalAttack();

	ENormalMinionStates GetState() { return m_CurState; }
	float GetNormalAttackRange() { return m_NormalAttackRange; }
	void SetState(ENormalMinionStates state);

protected:
	virtual void BeginPlay() override;
	virtual void SetHitState() override;
	

private:
	void initComponents();
	void initCollisions();
	void initAttackInformations();
	void updateState();
	
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void onNormalAttackMontageEnded();
	void onHitMontageEnded();
	void Die();
	
public:
	static int TagCount;

private:
	ENormalMinionStates m_CurState;

	TWeakObjectPtr<class UMeleeMinionAnim> m_ABPAnimInstance;
	
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* m_HitCollider;

	

};
