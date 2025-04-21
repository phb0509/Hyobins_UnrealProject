// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/SkillComponent.h"
#include "MainPlayerSkillComponent.generated.h"


UCLASS()
class HYOBINSPROJECT_API UMainPlayerSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	UMainPlayerSkillComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Default_OnGround
	void NormalAttack_OnGround();
	void UpperAttack_OnGround();
	void DashAttack_OnGround();
	void Dodge_OnGround();
	void Guard_OnGround();
	void Charging_OnGround();

	// Default_InAir
	void NormalAttack_InAir();
	void DashAttack_InAir();
	void EarthStrike_InAir();

	// Charging_OnGround
	void ChargingCancel_OnGround();
	void Charging_ComboDashAttack_OnGround();

	FORCEINLINE void ActivateStrikeAttack() { m_bIsStrikeAttackActive = true; }
	void DeactivateStrikeAttack() { ExtendStrikeActivateDecisionTime(); }
	
	void ExtendStrikeActivateDecisionTime(); // 강공격키판정을 좀 더 여유롭게하기위한 타이머호출 함수.
	
	
	FORCEINLINE bool IsStrikeAttackActive() const { return m_bIsStrikeAttackActive; }
	FORCEINLINE bool HasStartedComboKeyInputCheck() const { return m_bHasStartedComboKeyInputCheck; }
	FORCEINLINE bool CanChargingSkill() const { return m_bCanChargingSkill; }
	
	FORCEINLINE void SetHasStartedComboKeyInputCheck(const bool bHasStartedKeyInputCheck) { m_bHasStartedComboKeyInputCheck = bHasStartedKeyInputCheck;}
	FORCEINLINE void SetCanChargingSkill(bool bCanChargingSkill)  { m_bCanChargingSkill = bCanChargingSkill; }
	
	UFUNCTION()
	void SetIdle(UAnimMontage* Montage, bool bInterrupted);

	FORCEINLINE FTimerHandle& GetChargingTimer() { return m_ChargingTimer; }
	
private:
	void bindFuncOnMontageEvent();
	
	

private:
	bool m_bCanChargingSkill;
	
	bool m_bHasStartedComboKeyInputCheck;
	bool m_bIsStrikeAttackActive;
	
	UPROPERTY(EditAnywhere, Category = "StrikeAttackDecision")
	float m_StrikeAttackDecisionTime;

	FTimerHandle m_ChargingTimer;
};

