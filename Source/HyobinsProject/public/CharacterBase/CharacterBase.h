// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
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
class UMotionWarpingComponent;
class UCrowdControlComponent;

struct FHitInformation;
struct FAttackInformation;
enum class ECrowdControlType : uint8;

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
	void OnDamageStamina(const float staminaDamage) const;
	
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

	void SetInvincible(const bool bIsInvincible);
	
	// Get
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE UShapeComponent* GetCollider(const FName& colliderName) const;

	FORCEINLINE bool IsCrowdControlState() const;
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	
	FORCEINLINE bool IsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool IsOnGround() const { return m_bIsOnGround; }
	FORCEINLINE bool IsFalling() const { return m_bIsFalling; }
	FORCEINLINE bool IsFlying() const { return m_bIsFlying; }
	FORCEINLINE bool IsDead() const { return m_bIsDead; }

	FORCEINLINE void SetIsSuperArmor(const bool bIsSuperArmor) { m_bIsSuperArmor = bIsSuperArmor; }
	
	void RotateToTarget(const AActor* target, const FRotator& rotatorOffset = {0.0f, 0.0f ,0.0f});
	bool IsWithInRange(const AActor* target, const float range) const;
	FVector GetDirectionToTarget(const AActor* target) const;
	
	bool HasEnoughStamina(const float cost) const;

	UAnimInstanceBase* GetAnimInstanceBase() const;
	UMotionWarpingComponent* GetMotionWarpingComponent() const;

	void BreakCrowdControlState();
	
protected:
	virtual void execEvent_CommonCrowdControl(AActor* instigator) {};
	
	virtual void OnHPIsZero();
	virtual void OnStaminaIsZero() {};

	virtual void PlayOnHitEffect(const FHitInformation& hitInformation);
	void ApplyKnockback(const float knockbackDistance, const FVector& instigatorLocation);
	
	// Death
	UFUNCTION()
	virtual void Die();
	
	virtual void ExecEvent_EndedDeathMontage() {};
	virtual void OnCalledNotify_End_Death();

private:
	void playDeathMontage(const int32 hitDirection);
	
public:
	FOnTakeDamageDelegate OnTakeDamage;
	FOnDeathDelegate OnDeath;
	
	static const FName HitColliderName;
	static const FName KnockbackMontageNames[4];
	static const FName DeathMontageNames[4];
	
protected:
	
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
	bool m_bIsDead;

	TMap<FName, TSet<TWeakObjectPtr<AActor>>> m_HitActorsByMe;
	
	TMap<FName, TWeakObjectPtr<UShapeComponent>> m_Colliders;

	TWeakObjectPtr<UAnimInstanceBase> m_AnimInstanceBase;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStatComponent> m_StatComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCrowdControlComponent> m_CrowdControlComponent;
	
	UPROPERTY(EditAnywhere) 
    TObjectPtr<UCapsuleComponent> m_HitCollider;
	
	int32 m_LastHitDirection;
	
	UPROPERTY(EditAnywhere, Category = "HitEffect")
	TObjectPtr<UNiagaraSystem> m_HitEffect;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> m_HitSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionWarpingComponent> m_MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Default Montages")
	TArray<TObjectPtr<UAnimMontage>> m_DeathMontages;
	
private:
	FName m_LastPlayedOnHitMontageName;
	
};

