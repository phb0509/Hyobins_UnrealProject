// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/SkillComponent.h"
#include "MainPlayerSkillComponent.generated.h"

class AMainPlayer;
class UMainPlayerAnim;

enum class EMainPlayerSkillStates : uint8;


UCLASS()
class HYOBINSPROJECT_API UMainPlayerSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	UMainPlayerSkillComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual FName GetHighestPriorityInputMappingContext() override;
	
	void NormalAttack_OnGround();
	void UpperAttack_OnGround();
	void DashAttack_OnGround();
	void Dodge_OnGround();
	
	void NormalAttack_InAir();
	void DashAttack_InAir();
	void EarthStrike_InAir();
	void Charging_OnGround();
	void Charging_ComboDashAttack_OnGround();

	FORCEINLINE void ActivateStrikeAttack() { m_bIsStrikeAttackActive = true; }
	void DeactivateStrikeAttack() { ExtendStrikeActivateDecisionTime(); }
	
	void ExtendStrikeActivateDecisionTime(); // 강공격키판정을 좀 더 여유롭게하기위한 타이머호출 함수.
	
	
	FORCEINLINE EMainPlayerSkillStates GetState() const { return m_CurSkillState; }
	FORCEINLINE bool GetIsStrikeAttackActive() const { return m_bIsStrikeAttackActive; }
	FORCEINLINE bool GetHasStartedComboKeyInputCheck() const { return m_bHasStartedComboKeyInputCheck; }
	FORCEINLINE bool GetCanDodge() const { return m_bCanDodge; }
	FORCEINLINE bool GetCanChargingSkill() const { return m_bCanChargingSkill; }
	FORCEINLINE float GetGravityScaleInAir() const { return m_GravityScaleInAir; }

	FORCEINLINE void SetSkillState(EMainPlayerSkillStates state) { m_CurSkillState = state; }
	FORCEINLINE void SetHasStartedComboKeyInputCheck(const bool bHasStartedKeyInputCheck) { m_bHasStartedComboKeyInputCheck = bHasStartedKeyInputCheck;}
	FORCEINLINE void SetCanDodge(bool bCanDodge) { m_bCanDodge = bCanDodge; }
	FORCEINLINE void SetCanChargingSkill(bool bCanChargingSkill)  { m_bCanChargingSkill = bCanChargingSkill; }
	
	UFUNCTION()
	void SetIdle(UAnimMontage* Montage, bool bInterrupted);

	void InitGravityScaleAfterAttack(); // 특정공격들(공중에 유지시키기위해 중력값을 약하게 만들어놓는) 이후 다시 정상값으로 초기화.
	
private:
	void initSkills();
	void bindFuncOnMontageEvent();
	bool canNonChargingSkill_OnGround() const;
	


private:
	TWeakObjectPtr<AMainPlayer> m_Owner;
	TWeakObjectPtr<UMainPlayerAnim> m_OwnerAnimInstance;
	EMainPlayerSkillStates m_CurSkillState;
	
	UPROPERTY(EditAnywhere, Category = "GravityScaleInAir")
	float m_GravityScaleInAir;
	
	bool m_bCanDodge;
	bool m_bCanChargingSkill;
	
	bool m_bHasStartedComboKeyInputCheck;
	bool m_bIsStrikeAttackActive;
	
	UPROPERTY(EditAnywhere, Category = "StrikeAttackDecision")
	float m_StrikeAttackDecisionTime;
};

