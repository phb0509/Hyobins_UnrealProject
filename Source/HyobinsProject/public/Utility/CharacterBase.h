// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/CustomStructs.h"
#include "Kismet/GameplayStatics.h"
#include "HPGameInstance.h"
#include "Utility/PoolableActor.h"
#include "CharacterBase.generated.h"

UCLASS()
class HYOBINSPROJECT_API ACharacterBase : public ACharacter, public IPoolableActor
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void PossessedBy(AController* newController) override;

	float GetCurSpeed() { return m_CurSpeed; }
	bool GetIsIdle() { return m_bIsIdle; }
	bool GetIsWalking() { return m_bIsWalking; }
	bool GetIsRunning() { return m_bIsRunning; }
	bool GetIsAttacking() { return m_bIsAttacking; }
	bool GetIsInAir() { return m_bIsInAir; }
	bool GetIsDeath() { return m_bIsDeath; }

	virtual void OnHitTimerEnded() {};
	virtual void DeathTimerEnded();

	virtual void OnCalledDeathMontageEndedNotify();

protected:
	void initAttackInformations(FString path);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() {};

	virtual void SetHitState() {};
	virtual void ExecHitEvent(ACharacterBase* instigator) {};

	virtual void Activate() override;
	virtual void DeActivate() override;
	
protected:
	TMap<FName, FAttackInfoStruct> m_AttackInformations;
	
	UPROPERTY()
		TArray<class UCapsuleComponent*> m_HitColliders;

	FTimerHandle m_OnHitTimerHandle;
	FTimerHandle m_DeathTimerHandle;

	float m_MaxHP;
	float m_CurHP;
	float m_CurSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_HitRecovery;
	float m_OnHitTimerTime;
	float m_DeathTimerTime;

	bool m_bIsActivated;
	bool m_bIsIdle;
	bool m_bIsWalking;
	bool m_bIsRunning;
	bool m_bIsAttacking;
	bool m_bIsInAir;
	bool m_bIsSuperArmor;
	bool m_bIsDeath;
};
