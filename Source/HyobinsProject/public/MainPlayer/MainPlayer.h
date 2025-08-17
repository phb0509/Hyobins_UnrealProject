// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "MainPlayer.generated.h"


struct FInputActionValue;
class UMainPlayerSkillComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UMotionWarpingComponent;
class UInputMappingContext;
class UInputAction;
class UParticleSystem;


enum class EMainPlayerStates : uint8;
enum class ETriggerEvent : uint8;


UCLASS()
class HYOBINSPROJECT_API AMainPlayer final: public APlayableCharacter
{
	GENERATED_BODY()

public:
	AMainPlayer();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Attack(const FName& attackName, AActor* target, const FVector& causerLocation) override;
	virtual void OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation*, AActor* instigator, const FVector& causerLocation) override;
	virtual void PlayOnHitEffect(const FHitInformation& hitInformation) override;

	FORCEINLINE bool IsParrying() const { return m_bIsParrying; }
	FORCEINLINE void SetIsParrying(const bool bIsParrying) { m_bIsParrying = bIsParrying; }

	
private:
	void initAssets();
	void printLog() const;
	void playAttackEffect();
	void printLog();
	
public:
	static const FName SwordColliderName;
	static const FName ShieldForAttackColliderName;
	static const FName ShieldForGuardColliderName;

	
private:
	bool m_bIsParrying;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true)) 
	TObjectPtr<UCapsuleComponent> m_SwordCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> m_ShieldForAttackCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> m_ShieldCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true)) 
	TObjectPtr<UCapsuleComponent> m_ShieldBottomCollider;
	
	UPROPERTY(EditAnywhere, Category = "Parrying Effect")
    TObjectPtr<UParticleSystem> m_ParryingHitParticle;
};
