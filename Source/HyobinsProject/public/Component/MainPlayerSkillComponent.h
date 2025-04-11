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
	
	void NormalAttack_OnGround();
	void UpperAttack_OnGround();
	void DashAttack_OnGround();
	void Dodge_OnGround();
	void Guard_OnGround();
	
	void NormalAttack_InAir();
	void DashAttack_InAir();
	void EarthStrike_InAir();
	void Charging_OnGround();
	void Charging_ComboDashAttack_OnGround();

	FORCEINLINE void ActivateStrikeAttack() { m_bIsStrikeAttackActive = true; }
	void DeactivateStrikeAttack() { ExtendStrikeActivateDecisionTime(); }
	
	void ExtendStrikeActivateDecisionTime(); // 강공격키판정을 좀 더 여유롭게하기위한 타이머호출 함수.

	
	FORCEINLINE bool GetIsStrikeAttackActive() const { return m_bIsStrikeAttackActive; }
	FORCEINLINE bool GetHasStartedComboKeyInputCheck() const { return m_bHasStartedComboKeyInputCheck; }
	FORCEINLINE bool GetCanDodge() const { return m_bCanDodge; }
	FORCEINLINE bool GetCanChargingSkill() const { return m_bCanChargingSkill; }
	

	
	FORCEINLINE void SetHasStartedComboKeyInputCheck(const bool bHasStartedKeyInputCheck) { m_bHasStartedComboKeyInputCheck = bHasStartedKeyInputCheck;}
	FORCEINLINE void SetCanDodge(bool bCanDodge) { m_bCanDodge = bCanDodge; }
	FORCEINLINE void SetCanChargingSkill(bool bCanChargingSkill)  { m_bCanChargingSkill = bCanChargingSkill; }
	
	UFUNCTION()
	void SetIdle(UAnimMontage* Montage, bool bInterrupted);

	
private:
	void bindFuncOnMontageEvent();
	
	

private:
	
	bool m_bCanDodge;
	bool m_bCanChargingSkill;
	
	bool m_bHasStartedComboKeyInputCheck;
	bool m_bIsStrikeAttackActive;
	
	UPROPERTY(EditAnywhere, Category = "StrikeAttackDecision")
	float m_StrikeAttackDecisionTime;
};

