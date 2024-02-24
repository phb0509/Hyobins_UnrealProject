// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ActorPool/PoolableActor.h"
#include "Utility/CustomStructs.h"
#include "Utility/StatActor.h"
#include "CharacterBase.generated.h"

enum class EMonsterCommonStates : uint8;

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter, public IPoolableActor, public IStatActor
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void PossessedBy(AController* newController) override;

	virtual void OnHitTimerEnded() {};
	virtual void OnCalledDeathMontageEndedNotify();

	virtual void SetCommonState(EMonsterCommonStates commonState) {};

	// Get
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetHitRecovery() const { return m_HitRecovery; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsIdle() const { return m_bIsIdle; }
	FORCEINLINE bool GetIsWalking() const { return m_bIsWalking; }
	FORCEINLINE bool GetIsRunning() const { return m_bIsRunning; }
	FORCEINLINE bool GetIsAttacking() const { return m_bIsAttacking; }
	FORCEINLINE bool GetIsInAir() const { return m_bIsInAir; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsDeath() const { return m_bIsDeath; }
	FORCEINLINE bool GetIsHitStateTrigger() const { return m_bIsHitStateTrigger; }
	FORCEINLINE int32 GetHitDirection() const { return m_HitDirection; }
	
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die() {};
	virtual void SetHitState() {};
	virtual void OnHPIsZero();
	virtual void ExecHitEvent(ACharacterBase* instigator){};
	virtual void ExecDeathEvent() {};

	// IPoolableActor
	virtual void Initialize() override {};
	virtual void Activate() override {};
	virtual void DeActivate() override {};


protected:
	TWeakObjectPtr<class AAIControllerBase> m_AIControllerBase;
	TWeakObjectPtr<class UAnimInstanceBase> m_AnimInstanceBase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Colliders, Meta = (AllowProtectedAccess = true))
	TArray<UShapeComponent*> m_HitColliders;

	TMap<FName, TMap<AActor*, bool>> m_CheckHitActors;

	FTimerHandle m_OnHitTimerHandle;
	FTimerHandle m_DeathTimerHandle;
	FTimerHandle m_DeActivateTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	float m_WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float m_RunSpeed;

	UPROPERTY(EditDefaultsOnly)
	float m_HitRecovery;

	UPROPERTY(EditDefaultsOnly)
	float m_OnHitTimerTime;

	UPROPERTY(EditDefaultsOnly)
	float m_DeathTimerTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	float m_CurSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsIdle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsWalking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsRunning;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsAttacking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsSuperArmor;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsDeath;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsHitStateTrigger; // 몬스터들 연속피격모션 재생을위한 변수 (피격모션을 블렌드스페이스로 재생하기때문.)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	int32 m_HitDirection;
	
	float m_DeathTimerTickTime;
	float m_DeathTimerRemainingTime;
	float m_DiffuseRatio;
};
