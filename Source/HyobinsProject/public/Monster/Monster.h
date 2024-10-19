// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "ActorPool/PoolableActor.h"
#include "Templates/IsIntegral.h"
#include "Templates/IsEnumClass.h"
#include "Monster.generated.h"


UCLASS(Abstract)
class HYOBINSPROJECT_API AMonster : public ACharacterBase,  public IPoolableActor
{
	GENERATED_BODY()

public:
	AMonster();
	virtual void BeginPlay() override;

	template<typename T>
	typename TEnableIf<(TIsEnumClass<T>::Value || TIsIntegral<T>::Value), void>::Type
	SetFSMState(const T state)
	{
		uint8 stateIndex = static_cast<uint8>(state);
		m_CurFSMState = stateIndex;
		setFSMStateAsBehaviorTree(stateIndex);
	}
	
protected:
	virtual void execEvent_CommonCrowdControl(const ACharacterBase* instigator) override;
	virtual void SetCrowdControlState(ECrowdControlStates state) override;
	
	// IPoolableActor VirtualFunction
	virtual void Initialize() override;
	virtual void Activate() override;
	virtual void DeActivate() override;
	
private:
	void activateHitEffect(const FHitInformation&);
	void setFSMStateAsBehaviorTree(uint8 stateIndex) const;
		
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName EnemyKey;
	static const FName FSMStateKey;
	static const FName CrowdControlStateKet;
	static const FName NormalAttackSpeedKey;

protected:
	uint8 m_CurFSMState;
	FTimerHandle m_DiffuseRatioOnHitTimer;
	
private:
	TWeakObjectPtr<AAIControllerBase> m_AIControllerBase;
	
};
