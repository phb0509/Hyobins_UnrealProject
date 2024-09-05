// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/TimelineComponent.h"
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "CharacterBase.generated.h"

class UStatComponent;
class AAIControllerBase;
class UAIPerceptionComponent;
class UShapeComponent;

struct FAttackInfo;
enum class ECrowdControlType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCrowdControl_Start_Delegate, ACharacterBase*, const FAttackInfo*);
DECLARE_MULTICAST_DELEGATE(FOnCrowdControl_End_Delegate);

UCLASS(abstract)
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
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
	FORCEINLINE UAIPerceptionComponent* GetAIPerceptionComponent() const { return m_AIPerceptionComponent; }
	FORCEINLINE float GetWalkSpeed() const { return m_WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return m_RunSpeed; }
	FORCEINLINE float GetCurSpeed() const { return m_CurSpeed; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE bool GetIsOnGround() const { return m_bIsOnGround; }
	FORCEINLINE bool GetIsFalling() const { return m_bIsFalling; }
	FORCEINLINE bool GetIsFlying() const { return m_bIsFlying; }
	
	FORCEINLINE bool GetIsDead() const { return m_bIsDead; }
	FORCEINLINE UShapeComponent* GetCollider(const FName& colliderName) { return m_Colliders[colliderName].Get(); }
	
protected:
	
	virtual void ExecEvent_TakeKnockbackAttack(ACharacterBase* instigator, const FAttackInfo* attackInfo){};
	virtual void OnCalledTimer_KnockbackOnStanding_End() {};
	virtual void OnCalledTimer_KnockbackInAir_Loop() {};
	virtual void OnCalledTimer_KnockbackInAir_End() {};

	virtual void ExecEvent_TakeAirborneAttack(ACharacterBase* instigator, const FAttackInfo* attackInfo){};
	virtual void OnCalledTimer_Airborne_Loop() {};
	
	virtual void ExecEvent_TakeGroggyAttack(ACharacterBase* instigator, const FAttackInfo* attackInfo){};
	virtual void OnCalledTimer_Groggy_End() {};
	
	virtual void ExecEvent_OnHPIsZero();

	UFUNCTION()
	virtual void Die() {};
	virtual void OnCalledNotify_End_Death(); // �����Ÿ����� �Ϸ�� ȣ��.
	virtual void OnCalledNotify_End_GetUp() {};
	virtual void ExecEvent_EndedDeathMontage() {};
	virtual void OnCalledTimer_EndedDeathMontage() {}; 

private:
	virtual void execEvent_CommonCrowdControl(ACharacterBase* instigator) {};

	
protected:
	TMap<FName, TMap<TWeakObjectPtr<AActor>, bool>> m_HitActorsByMe;
	TMap<FName, TWeakObjectPtr<UShapeComponent>> m_Colliders;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStatComponent* m_StatComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UAIPerceptionComponent* m_AIPerceptionComponent;

	TMap<ECrowdControlType, FOnCrowdControl_Start_Delegate> m_CrowdControl_Start_Delegates;
	TMap<ECrowdControlType, FOnCrowdControl_End_Delegate> m_CrowdControl_End_Delegates;
	
	FTimerHandle m_CrowdControlTimerHandle;
	float m_CrowdControlTime;
	
	int32 m_HitDirection;
	
	FTimeline m_DeathTimeline;					// Timeline 생성

	UPROPERTY(EditAnywhere, Category = "Timeline | Death")
	UCurveFloat* m_DeathCurveFloat;	
	
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
	
};
