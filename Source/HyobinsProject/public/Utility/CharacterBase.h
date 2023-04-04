// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/CustomStructs.h"
#include "Kismet/GameplayStatics.h"
#include "HPGameInstance.h"
#include "CharacterBase.generated.h"

UCLASS()
class HYOBINSPROJECT_API ACharacterBase : public ACharacter
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

	virtual void OnHitTimerEnded();

protected:
	void LoadMesh(FString assetPath);
	void LoadAnimInstance(FString assetPath);
	void initAttackInformations(FString path);

	virtual void SetHitState() {};
	virtual void Die();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	
	
protected:
	TMap<FName, FAttackInfoStruct> m_AttackInformations;
	FTimerHandle m_OnHitTimerHandle;

	float m_MaxHP;
	float m_CurHP;
	float m_CurSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_HitRecovery;
	float m_OnHitTimer;

	bool m_bIsActivated;
	bool m_bIsIdle;
	bool m_bIsWalking;
	bool m_bIsRunning;
	bool m_bIsAttacking;
	bool m_bIsInAir;
	bool m_bIsSuperArmor;
	bool m_bIsDeath;
};
