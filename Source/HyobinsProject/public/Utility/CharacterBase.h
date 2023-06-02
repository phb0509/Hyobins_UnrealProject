// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CustomStructs.h"
#include "Utility/PoolableActor.h"
#include "CharacterBase.generated.h"

enum class EMonsterCommonStates : uint8;

UCLASS()
class HYOBINSPROJECT_API ACharacterBase : public ACharacter, public IPoolableActor
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void PossessedBy(AController* newController) override;
	virtual void OnHitTimerEnded() {};
	virtual void OnCalledDeathMontageEndedNotify();

	float GetCurSpeed() const { return m_CurSpeed; }
	bool GetIsIdle() const { return m_bIsIdle; }
	bool GetIsWalking() const { return m_bIsWalking; }
	bool GetIsRunning() const { return m_bIsRunning; }
	bool GetIsAttacking() const { return m_bIsAttacking; }
	bool GetIsInAir() const { return m_bIsInAir; }
	bool GetIsDeath() const { return m_bIsDeath; }

	virtual void SetCommonState(EMonsterCommonStates commonState) {};
	void InitHP(float hp) { m_CurHP = hp; m_MaxHP = m_CurHP; }

protected:
	void initAttackInformations(FString path);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() {};

	virtual void SetHitState() {};
	virtual void ExecHitEvent(ACharacterBase* instigator) {};
	virtual void ExecDeathEvent() {};

	virtual void Activate() {};
	virtual void DeActivate() {};
	
protected:
	TWeakObjectPtr<class AAIControllerBase> m_AIControllerBase;
	TWeakObjectPtr<class UAnimInstanceBase> m_AnimInstanceBase;
	TMap<FName, FAttackInfoStruct> m_AttackInformations;
	
	UPROPERTY()
		TArray<class UShapeComponent*> m_HitColliders;

	FTimerHandle m_OnHitTimerHandle;
	FTimerHandle m_DeathTimerHandle;
	FTimerHandle m_DeActivateTimerHandle;

	float m_MaxHP;
	float m_CurHP;
	float m_CurSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_HitRecovery;
	float m_OnHitTimerTime;
	float m_DeathTimerTime;
	float m_DeathTimerTickTime;
	float m_DeathTimerRemainingTime;
	float m_DiffuseRatio;

	bool m_bIsIdle;
	bool m_bIsWalking;
	bool m_bIsRunning;
	bool m_bIsAttacking;
	bool m_bIsInAir;
	bool m_bIsSuperArmor;
	bool m_bIsDeath;
};
