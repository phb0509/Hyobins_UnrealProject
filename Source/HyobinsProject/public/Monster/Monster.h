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

	virtual void SetCommonState(int32 commonState) {};
	FORCEINLINE float GetPatrolRange() const { return m_PatrolRange; }
	FORCEINLINE float GetNormalAttackRange() const { return m_NormalAttackRange; }

	
protected:
	virtual void ExecOnHitEvent(ACharacterBase* instigator) override;
	
	// IPoolableActor VirtualFunction
	virtual void Initialize() override;
	virtual void Activate() override;
	virtual void DeActivate() override;


public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName EnemyKey;
	static const FName StateKey;
	static const FName NormalAttackSpeedKey;


protected:
	UPROPERTY(EditDefaultsOnly) // SkillComponent쪽으로 결국 전부옮김.
	float m_PatrolRange;

	UPROPERTY(EditDefaultsOnly)
	float m_NormalAttackRange;

	UPROPERTY(EditDefaultsOnly)
	float m_NormalAttackSpeed;

};
