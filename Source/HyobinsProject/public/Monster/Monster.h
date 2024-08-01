// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "ActorPool/PoolableActor.h"
#include "Monster.generated.h"


UCLASS()
class HYOBINSPROJECT_API AMonster : public ACharacterBase,  public IPoolableActor
{
	GENERATED_BODY()

public:
	AMonster();
	virtual void BeginPlay() override;
	
	virtual void SetCommonState(int32 commonState) {};


protected:
	// IPoolableActor VirtualFunction
	virtual void Initialize() override;
	virtual void Activate() override;
	virtual void DeActivate() override;
	
	virtual void ExecEvent_Knockback(ACharacterBase* instigator) override;
	virtual void ExecEvent_Groggy(ACharacterBase* instigator) override;
	

public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName EnemyKey;
	static const FName StateKey;
	static const FName NormalAttackSpeedKey;

protected:
	float m_DeathTimerTickTime;
	float m_DeathTimerRemainingTime;
	float m_DiffuseRatio;
	
private:
	TWeakObjectPtr<AAIControllerBase> m_AIControllerBase;
	
};
