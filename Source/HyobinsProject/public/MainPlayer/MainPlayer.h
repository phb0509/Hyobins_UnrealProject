// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter//PlayableCharacter.h"
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

// DECLARE_MULTICAST_DELEGATE(FOnChangeInputMappingContextDelegate);

UCLASS()
class HYOBINSPROJECT_API AMainPlayer final: public APlayableCharacter
{
	GENERATED_BODY()

public:
	AMainPlayer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Attack(const FName& attackName, TWeakObjectPtr<AActor> target) override;
	virtual void PlayOnHitEffect(const FHitInformation& hitInformation) override;

	
	// AxisMappings
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	
	FORCEINLINE void InitArrowKeys()
	{
		m_CurInputHorizontal = 0;
		m_CurInputVertical = 0;
	}
	
	// ActionMappings
	void Run() const;
	void StopRun() const;


	UFUNCTION(BlueprintCallable, Category = "InputMappingContext")
	void AddInputContextMappingInAir();

	UFUNCTION(BlueprintCallable, Category = "InputMappingContext")
	void RemoveInputContextMappingInAir();
	
	void AddInputContextMappingOnCharging();
	void RemoveInputContextMappingOnCharging();
	
	// Get
	FORCEINLINE UMainPlayerSkillComponent* GetSkillComponent() const { return m_SkillComponent; }


	
private:
	void initAssets();
	void printLog() const;
	void playAttackEffect();
	
public:
	static const FName SwordColliderName;
	static const FName ShieldForAttackColliderName;
	static const FName ShieldForDefendColliderName;
	
	
	//FOnChangeInputMappingContextDelegate OnChangeInputMappingContext;

private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMainPlayerSkillComponent> m_SkillComponent;
	
	UPROPERTY(EditAnywhere) 
	TObjectPtr<UCapsuleComponent> m_SwordCollider;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_ShieldForAttackCollider;
    
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_ShieldForDefendCollider;

	UPROPERTY(EditAnywhere) 
	TObjectPtr<UCapsuleComponent> m_ShieldBottomCollider;
	
	
	// KeyInput
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | OnGround", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> m_InputMappingContextOnGround;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | OnGround", Meta = (AllowPrivateAccess = true))
	TMap<FName, TObjectPtr<UInputAction>> m_InputActionsDefaultOnGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | InAir", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> m_InputMappingContextInAir;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | InAir", Meta = (AllowPrivateAccess = true))
	TMap<FName, TObjectPtr<UInputAction>> m_InputActionsInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | Charging", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> m_InputMappingContextOnCharging;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | Charging", Meta = (AllowPrivateAccess = true))
	TMap<FName, TObjectPtr<UInputAction>> m_InputActionsOnCharging;
	
	
	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bIsPressedShift;

	
	
};
