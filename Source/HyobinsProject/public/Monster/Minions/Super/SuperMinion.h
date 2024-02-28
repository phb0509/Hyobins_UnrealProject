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

	FORCEINLINE ENormalMinionStates GetState() const { return m_CurState; }
	void SetState(ENormalMinionStates state);

protected:
	virtual void BeginPlay() override;
	virtual void SetCommonState(const EMonsterCommonStates commonState) override;
	virtual void Die() override;
	virtual void ExecDeathEvent() override;

	void OnDeathEventTimerEnded();


private:
	void initAssets();
	void updateState();
	void onNormalAttackMontageEnded();

	UFUNCTION()
		void onMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	static int TagCount;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ENormalMinionStates m_CurState;

	TMap<FName, FAttackInfoStruct> m_AttackInformations;
	TWeakObjectPtr<class USuperMinionAnim> m_AnimInstance;
	TWeakObjectPtr<class ASuperMinionAIController> m_OwnerAIController;
};
