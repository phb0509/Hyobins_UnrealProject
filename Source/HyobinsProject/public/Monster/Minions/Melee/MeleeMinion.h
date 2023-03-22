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

	void NormalAttack();


	ENormalMinionStates GetState() { return m_CurState; }
	void SetState(ENormalMinionStates state) { m_CurState = state; }
	float GetNormalAttackRange() { return m_NormalAttackRange; }


protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void initComponents();
	void initCollisions();
	void initAttackInformations();
	void updateState();

	UFUNCTION()
		void OnNormalAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
public:
	static int TagCount;

private:
	ENormalMinionStates m_CurState;

	TWeakObjectPtr<class UMeleeMinionAnim> m_ABPAnimInstance;
	
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* m_HitCollider;

};
