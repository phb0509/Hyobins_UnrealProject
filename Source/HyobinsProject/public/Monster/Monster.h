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

	uint8 GetCurFSMState() const { return m_CurFSMState; }
	ACharacterBase* GetTarget() const;
	
	template<typename T>
	typename TEnableIf<(TIsEnumClass<T>::Value || TIsIntegral<T>::Value), void>::Type
	SetFSMState(const T state)
	{
		const uint8 stateIndex = static_cast<uint8>(state);
		m_CurFSMState = stateIndex;
		SetFSMStateAsBehaviorTree(stateIndex);
	}

	void SetFSMStateAsBehaviorTree(uint8 stateIndex) const;

	
protected:
	virtual void execEvent_CommonCrowdControl(const ACharacterBase* instigator) override;
	virtual void SetCrowdControlState(const ECrowdControlStates state) override;

	virtual void Die() override;
	virtual void ExecEvent_EndedDeathMontage() override;

	// 사망시 호출할 TimeLine 이벤트.
	UFUNCTION()
	void OnCalledTimelineEvent_Loop_AfterDeath(float curveValue); // 특정시간(조절 가능한)동안 디퓨즈값 검은색으로 점점 변환.

	UFUNCTION()
	void OnCalledTimelineEvent_End_AfterDeath(); // 타임라인 이벤트 종료시 호출(액터풀 회수직전 호출함수.)
	
	
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
	uint8 m_CurFSMState;
	FTimerHandle m_DiffuseRatioOnHitTimer;
	
private:
	TWeakObjectPtr<AAIControllerBase> m_AIControllerBase;
	
};
