// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrowdControlComponent.generated.h"

class ACharacterBase;
class AAIController;
class UAnimInstanceBase;
class UCharacterMovementComponent;
struct FHitInformation;
enum class ECrowdControlType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCrowdControl_Start_Delegate, AActor*, const FHitInformation&);


USTRUCT(Atomic)
struct FCrowdControlMontages
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Test")
	TArray<TObjectPtr<UAnimMontage>> montages;
};

USTRUCT(Atomic)
struct FCrowdControlSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "CrowdControl Montages")
	TMap<ECrowdControlType, FCrowdControlMontages> crowdControlMontages;
	
	UPROPERTY(EditAnywhere, Category = "CrowdControl Montages")
	TObjectPtr<UAnimMontage> getUpMontage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, AllowedClasses = "ACharacter"))
class HYOBINSPROJECT_API UCrowdControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrowdControlComponent();

	void ApplyCrowdControl(AActor* instigator, const FHitInformation& attackInfo);
	void ClearCrowdControlTimerHandle();
	void BreakCrowdControlState();
	
	FORCEINLINE ECrowdControlType GetCrowdControlState() const { return m_CurCrowdControlState; }
	FORCEINLINE bool IsCrowdControlState() const;
	void SetCrowdControlState(ECrowdControlType crowdControlType);
	
protected:
	virtual void BeginPlay() override;

	virtual void TakeAttack_Knockback(AActor* instigator, const FHitInformation& attackInfo);
	virtual void OnCalledTimer_KnockbackOnStanding_End();

	virtual void TakeAttack_Down(AActor* instigator, const FHitInformation& attackInfo);
	void CallTimer_CheckOnGround();
	virtual void CheckOnGround();
	virtual void GetUp();
	
	virtual void TakeAttack_Airborne(AActor* instigator, const FHitInformation& attackInfo);
	
	void DisableMovementComponentForDuration(float duration);
	

private:
	void applyKnockback(const FHitInformation& hitInfo);
	UCharacterMovementComponent* getCharacterMovementComponent();
	void playCrowdControlMontage(const ECrowdControlType crowdControlType, const int32 hitDirection);

private:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	TWeakObjectPtr<AAIController> m_OnwerAIController;
	TWeakObjectPtr<UAnimInstanceBase> m_OwnerAnimInstance;

	UPROPERTY(EditAnywhere, Category = "Setting")
	FCrowdControlSettings m_CrowdControlSettings;
	
	TWeakObjectPtr<UCharacterMovementComponent> m_CharacterMovementComponent;
	
	TMap<ECrowdControlType, FOnCrowdControl_Start_Delegate> m_CrowdControlStartDelegates;
	FTimerHandle m_CrowdControlTimerHandle;

	ECrowdControlType m_CurCrowdControlState;
	
	FName m_LastPlayedOnHitMontageName;

	float m_LastCrowdControlTime;
};
