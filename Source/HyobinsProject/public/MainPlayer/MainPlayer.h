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
class AMainPlayer;
class UInputMappingContext;
class UInputAction;

enum class EMainPlayerStates : uint8;
enum class ETriggerEvent : uint8;


UCLASS()
class HYOBINSPROJECT_API AMainPlayer final: public APlayableCharacter
{
	GENERATED_BODY()

public:
	AMainPlayer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Attack(const FName& attackName, AActor* target, const FVector& causerLocation) override;
	virtual void OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation*, AActor* instigator, const FVector& causerLocation) override;
	virtual void PlayOnHitEffect(const FHitInformation& hitInformation) override;

	
	// AxisMappings
	void Move(const FInputActionValue& value); // WSAD
	void Look(const FInputActionValue& value); // Mouse X,Y
	
	FORCEINLINE void InitArrowKeys()
	{
		m_CurInputHorizontal = 0;
		m_CurInputVertical = 0;
	}
	
	// ActionMappings
	void Run();
	void StopRun();
	
	bool IsGuard() const { return m_bIsGuarding; }
	void SetIsGuarding(bool bIsGuard) { m_bIsGuarding = bIsGuard; }

	void EnableGuard();
	void DisableGuard();

private:
	void initAssets();
	void printLog() const;
	void playAttackEffect();
	bool canGuard(const AActor* instigator) const;
	
public:
	static const FName SwordColliderName;
	static const FName ShieldForAttackColliderName;
	static const FName ShieldForGuardColliderName;

	
private:
	bool m_bIsGuarding;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true)) 
	TObjectPtr<UCapsuleComponent> m_SwordCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> m_ShieldForAttackCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> m_ShieldCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true)) 
	TObjectPtr<UCapsuleComponent> m_ShieldBottomCollider;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bIsPressedShift;
};
