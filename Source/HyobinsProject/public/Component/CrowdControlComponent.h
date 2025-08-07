// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/EnumTypes.h"
#include "CrowdControlComponent.generated.h"

class ACharacterBase;
class AAIController;
class UAnimInstanceBase;
class UCharacterMovementComponent;
struct FHitInformation;
enum class ECrowdControlType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCrowdControl_Start_Delegate, AActor*, const FHitInformation&);
DECLARE_MULTICAST_DELEGATE(FOnEndedGroggy);

USTRUCT(Atomic)
struct FCrowdControlMontages
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAnimMontage>> montages;
};

USTRUCT(Atomic)
struct FCrowdControlSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CrowdControl Montages")
	TMap<ECrowdControlType, FCrowdControlMontages> crowdControlMontages;
	
	UPROPERTY(EditDefaultsOnly, Category = "CrowdControl Montages")
	TObjectPtr<UAnimMontage> getUpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "CrowdControl Time")
	float groggyTime;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, AllowedClasses = "ACharacter"))
class HYOBINSPROJECT_API UCrowdControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrowdControlComponent();

	void ApplyCrowdControl(AActor* instigator, const FHitInformation& attackInfo);
	void Groggy();
	void Execution();
	void ClearCrowdControlTimerHandle();
	void BreakCrowdControlState();
	
	FORCEINLINE ECrowdControlType GetCrowdControlState() const { return m_CurCrowdControlState; }
	FORCEINLINE bool IsCrowdControlState() const;
	FORCEINLINE bool IsGroggy() const { return m_CurCrowdControlState == ECrowdControlType::Groggy; }
	FORCEINLINE float GetGroggyTime() const { return m_CrowdControlSetting.groggyTime; }
	
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

public:
	FOnEndedGroggy OnEndedGroggy;
	
private:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	TWeakObjectPtr<AAIController> m_OnwerAIController;
	TWeakObjectPtr<UAnimInstanceBase> m_OwnerAnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	FCrowdControlSettings m_CrowdControlSetting;
	
	TWeakObjectPtr<UCharacterMovementComponent> m_CharacterMovementComponent;
	
	TMap<ECrowdControlType, FOnCrowdControl_Start_Delegate> m_CrowdControlStartDelegates;
	FTimerHandle m_CrowdControlTimerHandle;

	ECrowdControlType m_CurCrowdControlState;
	
	FName m_LastPlayedOnHitMontageName;

	float m_LastCrowdControlTime;
};
