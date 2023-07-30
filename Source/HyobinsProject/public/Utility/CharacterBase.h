// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CustomStructs.h"
#include "Utility/PoolableActor.h"
#include "Component/StatComponent.h"
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

	virtual void SetCommonState(EMonsterCommonStates commonState) {};

protected:
	void initAttackInformations(FString path);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() {};

	virtual void SetHitState() {};
	virtual void OnHPIsZero();
	virtual void ExecHitEvent(ACharacterBase* instigator) {};
	virtual void ExecDeathEvent() {};

	virtual void Activate() {};
	virtual void DeActivate() {};

protected:
	TWeakObjectPtr<class AAIControllerBase> m_AIControllerBase;
	TWeakObjectPtr<class UAnimInstanceBase> m_AnimInstanceBase;
	TMap<FName, FAttackInfoStruct> m_AttackInformations;

	UPROPERTY(VIsibleAnywhere, Category = Stat)
		UStatComponent* m_StatComponent;

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
		bool m_bIsHitStateTrigger;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowProtectedAccess = true))
		int m_HitDirection;

	float m_DeathTimerTickTime;
	float m_DeathTimerRemainingTime;
	float m_DiffuseRatio;
};
