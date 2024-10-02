// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "ActorPool/PoolableActor.h"
#include "Monster.generated.h"

//class AAIControllerBase;

UCLASS()
class HYOBINSPROJECT_API AMonster : public ACharacterBase,  public IPoolableActor
{
	GENERATED_BODY()

public:
	AMonster();
	virtual void BeginPlay() override;
	

protected:
	virtual void execEvent_CommonCrowdControl(const ACharacterBase* instigator) override;
	virtual void SetCrowdControlState(ECrowdControlState state) override;
	
	// IPoolableActor VirtualFunction
	virtual void Initialize() override;
	virtual void Activate() override;
	virtual void DeActivate() override;
	
private:
	void activateHitEffect(const FHitInformation&);
		
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName EnemyKey;
	static const FName FSMStateKey;
	static const FName CrowdControlStateKet;
	static const FName NormalAttackSpeedKey;

protected:
	FTimerHandle m_DiffuseRatioOnHitTimer;
	
private:
	TWeakObjectPtr<AAIControllerBase> m_AIControllerBase;
	
};
