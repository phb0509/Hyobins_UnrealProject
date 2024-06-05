// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorPool/PoolableActor.h"
#include "CharacterBase.generated.h"

enum class EMonsterCommonStates : uint8;

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void PossessedBy(AController* newController) override;
	virtual void OnHitTimerEnded() {};
	virtual void OnCalledNotify_EndedDeath(); 
	
	// Get
	FORCEINLINE class UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetHitRecovery() const { return m_HitRecovery; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsIdle() const { return m_bIsIdle; }
	FORCEINLINE bool GetIsWalking() const { return m_bIsWalking; }
	FORCEINLINE bool GetIsRunning() const { return m_bIsRunning; }
	FORCEINLINE bool GetIsInAir() const { return m_bIsInAir; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsDead() const { return m_bIsDead; }
	FORCEINLINE bool GetIsHitStateTrigger() const { return m_bIsHitStateTrigger; }
	FORCEINLINE int32 GetHitDirection() const { return m_HitDirection; }

	
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ExecHitEvent(ACharacterBase* instigator){};
	virtual void OnHPIsZero();
	virtual void Die() {};
	virtual void ExecDeathEvent() {}; // ������Ÿ�� ����Ϸ� �� ����Ǵ� �Լ�. CharacterBase::OnCalledDeathMontageEnded�Լ��ȿ��� ȣ���Ѵ�.



protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = StatComponent)
	class UStatComponent* m_StatComponent;
	
	TWeakObjectPtr<class AAIControllerBase> m_AIControllerBase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Colliders, Meta = (AllowProtectedAccess = true))
	TArray<UShapeComponent*> m_HitColliders;
	
	FTimerHandle m_OnHitTimerHandle;
	FTimerHandle m_DeathTimerHandle;
	FTimerHandle m_DeActivateTimerHandle;
	
	UPROPERTY(EditDefaultsOnly) // ������(��������)���� ������ �� �ְ�..
	float m_WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float m_RunSpeed;

	UPROPERTY(EditDefaultsOnly)
	float m_HitRecovery;

	UPROPERTY(EditDefaultsOnly)
	float m_OnHitTimerTime;

	UPROPERTY(EditDefaultsOnly)
	float m_DeathTimerTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	float m_CurSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsIdle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsWalking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsRunning;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsInAir;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsSuperArmor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsDead;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsHitStateTrigger; // ���͵� �����ǰݸ�� ��������� ���� (�ǰݸ���� ���彺���̽��� ����ϱ⶧��.)

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	int32 m_HitDirection;
	
	float m_DeathTimerTickTime;
	float m_DeathTimerRemainingTime;
	float m_DiffuseRatio;
};
