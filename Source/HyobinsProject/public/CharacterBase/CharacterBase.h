// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/TimelineComponent.h"
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "Utility/EnumTypes.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/Attacker.h"
#include "CharacterBase.generated.h"

class ACharacterBase;
class UAnimInstanceBase;
class UStatComponent;
class AAIControllerBase;
class UAIPerceptionComponent;
class UNiagaraSystem;
class USoundCue;

struct FHitInformation;
struct FAttackInformation;
enum class ECrowdControlStates : uint8;
enum class ECrowdControlType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCrowdControl_Start_Delegate, AActor*, const FAttackInformation*);
DECLARE_MULTICAST_DELEGATE(FOnCrowdControl_End_Delegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamageDelegate, const FHitInformation&);
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter, public IAttacker, public IDamageable
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// IAttacker
	virtual void Attack(const FName& attackName, AActor* target, const FVector& causerLocation) override;
	
	// IDamageable
	virtual void OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation*, AActor* instigator, const FVector& causerLocation) override;

	
	void ClearCrowdControlTimerHandle();
	
	
	bool HasContainHitActor(const FName& attackName, AActor* hitActor)
	{
		return m_HitActorsByMe[attackName].Contains(hitActor);
	}
	
	void AddHitActorsByMe(const FName& attackName, AActor* hitActor)
	{
		m_HitActorsByMe[attackName].Add(hitActor);
	}

	void EmptyHitActorsByMe(const FName& attackName)
	{
		m_HitActorsByMe[attackName].Empty();
	}

	// Get
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE UShapeComponent* GetCollider(const FName& colliderName) { return m_Colliders[colliderName].Get(); }

	FORCEINLINE ECrowdControlStates GetCurCrowdControlState() const { return m_CurCrowdControlState; }
	FORCEINLINE bool IsCrowdControlState() const { return m_CurCrowdControlState != ECrowdControlStates::None; }
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	
	FORCEINLINE bool IsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool IsSuperArmorBuff() const { return m_bIsSuperArmorBuff; }
	FORCEINLINE bool IsOnGround() const { return m_bIsOnGround; }
	FORCEINLINE bool IsFalling() const { return m_bIsFalling; }
	FORCEINLINE bool IsFlying() const { return m_bIsFlying; }
	FORCEINLINE bool IsDead() const { return m_bIsDead; }

	FORCEINLINE void SetIsSuperArmor(bool bIsSuperArmor, bool bForce);
	FORCEINLINE void SetIsSuperArmorBuff(bool bIsSuperArmorBuff) { m_bIsSuperArmorBuff = bIsSuperArmorBuff; }
	
	void RotateToTarget(const AActor* target, const FRotator& rotatorOffset);

	virtual void SetCrowdControlState(ECrowdControlStates state)
	{
		m_CurCrowdControlState = state;
	}

	
protected:
	virtual void execEvent_CommonCrowdControl(AActor* instigator) {};
	
	virtual void ExecEvent_TakeKnockbackAttack(AActor* instigator, const FAttackInformation* attackInfo);
	virtual void OnCalledTimer_KnockbackOnStanding_End();

	virtual void ExecEvent_TakeAirborneAttack(AActor* instigator, const FAttackInformation* attackInfo);
	virtual void ExecEvent_Down_WhenOnGround();

	virtual void ExecEvent_TakeDownAttack(AActor* instigator, const FAttackInformation* attackInfo);
	virtual void OnCalledTimer_Down_End();

	void CallTimer_ExecDownEvent_WhenOnGround();
	void DisableMovementComponentForDuration(float duration) const;
	
	virtual void ExecEvent_OnHPIsZero();
	virtual void ExecEvent_OnStaminaIsZero() {};

	virtual void PlayOnHitEffect(const FHitInformation& hitInformation) {};
	void ApplyKnockback(const float knockbackDistance, const FVector& instigatorLocation);
	
	// Death
	UFUNCTION()
	virtual void Die();
	virtual void ExecEvent_EndedDeathMontage() {};
	virtual void OnCalledNotify_End_Death();
	
private:
	void playOnHitMontage(const FName& montageName);
	

	
public:
	FOnTakeDamageDelegate OnTakeDamage;
	FOnDeathDelegate OnDeath;
	
	static const FName HitColliderName;
	static const FName KnockbackMontageNames[4];
	static const FName DeathMontageNames[4];
	
protected:
	ECrowdControlStates m_CurCrowdControlState;
	
	UPROPERTY(EditDefaultsOnly) 
	float m_WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float m_RunSpeed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	float m_CurSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsOnGround;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsFalling;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsFlying;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsSuperArmor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsSuperArmorBuff;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
	bool m_bIsDead;

	TMap<FName, TSet<TWeakObjectPtr<AActor>>> m_HitActorsByMe;
	
	TMap<FName, TWeakObjectPtr<UShapeComponent>> m_Colliders;

	TWeakObjectPtr<UAnimInstanceBase> m_AnimInstanceBase;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStatComponent> m_StatComponent;
	
	UPROPERTY(EditAnywhere) 
    TObjectPtr<UCapsuleComponent> m_HitCollider;
	
	TMap<ECrowdControlType, FOnCrowdControl_Start_Delegate> m_CrowdControlStartDelegates;
	
	FTimerHandle m_CrowdControlTimerHandle;
	float m_CrowdControlTime;
	int32 m_HitDirection;
	
	FTimeline m_DeathTimeline;					

	UPROPERTY(EditAnywhere, Category = "Timeline | Death")
	TObjectPtr<UCurveFloat> m_DeathCurveFloat;
	
	UPROPERTY(EditAnywhere, Category = "HitEffect")
	TObjectPtr<UNiagaraSystem> m_HitEffect;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> m_HitSound;

	UPROPERTY(EditAnywhere)
	float m_GameSpeedDelay;

	UPROPERTY(EditAnywhere)
	float m_OnHitPlayRate;
	
private:
	FName m_LastPlayedOnHitMontageName;
	
};

