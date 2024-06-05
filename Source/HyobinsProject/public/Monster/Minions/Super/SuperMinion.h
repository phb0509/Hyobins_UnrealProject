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
	
	void NormalAttack();
	void OnEndedNormalAttack();
	virtual void OnHitTimerEnded() override;
	
	
	// Get
	FORCEINLINE ENormalMinionStates GetState() const { return m_CurState; }

	// Set
	void SetState(ENormalMinionStates state);

protected:
	virtual void BeginPlay() override;
	
	virtual void ExecHitEvent(ACharacterBase* instigator) override;
	virtual void Die() override;
	virtual void ExecDeathEvent() override;
	void OnCalled_DeathEvent();

	virtual void SetCommonState(const int32 commonStateIndex) override;

	// IPoolableActor VirtualFunction 
	virtual void Activate() override;

private:
	void initAssets();
	void updateState();
	

public:
	static int TagCount;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ENormalMinionStates m_CurState;
	
	TWeakObjectPtr<class USuperMinionAnim> m_AnimInstance;
	TWeakObjectPtr<class ASuperMinionAIController> m_OwnerAIController;
};
