// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SuperMinion.generated.h"

enum class EMonsterCommonStates : uint8;
enum class ENormalMinionStates : uint8;

UCLASS()
class HYOBINSPROJECT_API ASuperMinion final: public AMonster
{
	GENERATED_BODY()
	
public:
	ASuperMinion();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void OnHitTimerEnded() override;

	void NormalAttack();

	ENormalMinionStates GetState() { return m_CurState; }
	float GetNormalAttackRange() { return m_NormalAttackRange; }
	void SetState(ENormalMinionStates state);

protected:
	virtual void BeginPlay() override;
	virtual void SetCommonState(EMonsterCommonStates commonState);
	virtual void ExecHitEvent(ACharacterBase* instigator) override;
	virtual void Die() override;
	virtual void ExecDeathEvent() override;

	void OnDeathEventTimerEnded();


private:
	void initAssets();
	void updateState();

	void onNormalAttackMontageEnded();
	
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	static int TagCount;

private:
	ENormalMinionStates m_CurState;

	TWeakObjectPtr<class USuperMinionAnim> m_AnimInstance;
	TWeakObjectPtr<class ASuperMinionAIController> m_OwnerAIController;
};
