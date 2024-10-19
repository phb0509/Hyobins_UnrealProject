// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/TimelineComponent.h"
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "CharacterBase.generated.h"

class ACharacterBase;
class UAnimInstanceBase;
class UStatComponent;
class AAIControllerBase;
class UAIPerceptionComponent;
class UShapeComponent;
class UNiagaraSystem;

struct FHitInformation;
struct FAttackInformation;
enum class ECrowdControlStates : uint8;
enum class ECrowdControlType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCrowdControl_Start_Delegate, const ACharacterBase*, const FAttackInformation*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, const FHitInformation&);
DECLARE_MULTICAST_DELEGATE(FOnCrowdControl_End_Delegate);

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation*, const ACharacterBase* instigator);
	
	void Attack(const FName& attackName, TWeakObjectPtr<AActor> target);
	
	bool HasContainHitActor(const FName& attackName, AActor* hitActor)
	{
		return m_HitActorsByMe[attackName].Contains(hitActor);
	}
	
	void AddHitActorsByMe(const FName& attackName, AActor* hitActor)
	{
		m_HitActorsByMe[attackName].Add(hitActor,true);
	}

	void EmptyHitActorsByMe(const FName& attackName)
	{
		m_HitActorsByMe[attackName].Empty();
	}

	// Get
	FORCEINLINE UStatComponent* GetStatComponent() const { return m_StatComponent; }
	FORCEINLINE UShapeComponent* GetCollider(const FName& colliderName) { return m_Colliders[colliderName].Get(); }
	
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsOnGround() const { return m_bIsOnGround; }
	FORCEINLINE bool GetIsFalling() const { return m_bIsFalling; }
	FORCEINLINE bool GetIsFlying() const { return m_bIsFlying; }
	FORCEINLINE bool GetIsDead() const { return m_bIsDead; }
	
	

	
protected:
	virtual void ExecEvent_TakeKnockbackAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) {}
	virtual void OnCalledTimer_KnockbackOnStanding_End() {};

	virtual void ExecEvent_TakeAirborneAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) {}
	virtual void ExecEvent_Down_WhenOnGround() {};

	virtual void ExecEvent_TakeDownAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) {}
	virtual void OnCalledTimer_Down_End() {};
	
	virtual void ExecEvent_OnHPIsZero();

	// Death
	UFUNCTION()
	virtual void Die() {};
	virtual void ExecEvent_EndedDeathMontage() {};
	virtual void OnCalledTimer_EndedDeathMontage() {};
	
	virtual void OnCalledNotify_End_Death();
	
	

	virtual void SetCrowdControlState(ECrowdControlStates state)
	{
		m_CurCrowdControlState = state;
	}

private:
	virtual void execEvent_CommonCrowdControl(const ACharacterBase* instigator) {};

public:
	FOnTakeDamage OnTakeDamage;
	
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
	bool m_bIsDead;

	TMap<FName, TMap<TWeakObjectPtr<AActor>, bool>> m_HitActorsByMe;
	TMap<FName, TWeakObjectPtr<UShapeComponent>> m_Colliders;

	TWeakObjectPtr<UAnimInstanceBase> m_AnimInstanceBase;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStatComponent* m_StatComponent;
	
	TMap<ECrowdControlType, FOnCrowdControl_Start_Delegate> m_CrowdControl_Start_Delegates;
	
	FTimerHandle m_CrowdControlTimerHandle;
	float m_CrowdControlTime;
	int32 m_HitDirection;
	
	FTimeline m_DeathTimeline;					

	UPROPERTY(EditAnywhere, Category = "Timeline | Death")
	UCurveFloat* m_DeathCurveFloat;
	
	UPROPERTY(EditAnywhere, Category = "HitEffect")
	TObjectPtr<UNiagaraSystem> m_HitEffect;
};
