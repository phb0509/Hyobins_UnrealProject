// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "Component/SkillComponent.h"
#include "MainPlayerSkillComponent.generated.h"

class AMainPlayer;
class UMainPlayerAnim;
class UNiagaraSystem;
class UNiagaraComponent;
class UCameraShakeBase;
class USoundWave;

enum class EMainPlayerSkillStates : uint8;
struct FMontageFunc;


UCLASS()
class HYOBINSPROJECT_API UMainPlayerSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	UMainPlayerSkillComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void NormalAttack_OnGround();
	void NormalAttack_InAir();
	
	void UpperAttack_OnGround();
	
	void DashAttack_OnGround();
	void DashAttack_InAir();
	
	void EarthStrike_InAir();
	void EarthStrike_OnGround();
	void EarthStrike_HitCheck();
	void EarthStrike_PlayEffect();
	
	void Dodge_OnGround();

	FORCEINLINE void ActivateStrikeAttack() { m_bIsStrikeAttackActive = true; }
	void DeactivateStrikeAttack() { ExtendStrikeActivateDecisionTime(); }
	
	void ExtendStrikeActivateDecisionTime(); // 강공격키판정을 좀 더 여유롭게하기위한 타이머호출 함수.
	
	//NormalAttack
	UFUNCTION()
	void OnCalledNotify_NormalAttack_Start_EachSection();

	
	FORCEINLINE EMainPlayerSkillStates GetState() const { return m_CurSkillState; }
	FORCEINLINE void SetHasStartedComboKeyInputCheck(const bool bHasStartedKeyInputCheck) { m_bHasStartedComboKeyInputCheck = bHasStartedKeyInputCheck;}
	FORCEINLINE void SetState(EMainPlayerSkillStates state) { m_CurSkillState = state; }
	
	UFUNCTION()
	void SetIdle(UAnimMontage* Montage, bool bInterrupted);
	
	
private:
	void bindFuncOnMontageEvent();
	void linqNextNormalAttackOnGroundCombo();
	void linqNextNormalAttackInAirCombo();
	void initGravityScaleAfterAttack(); // 특정공격들(공중에 유지시키기위해 중력값을 약하게 만들어놓는) 이후 다시 정상값으로 초기화.
	

private:
	TWeakObjectPtr<AMainPlayer> m_Owner;
	TWeakObjectPtr<UMainPlayerAnim> m_OwnerAnimInstance;
	EMainPlayerSkillStates m_CurSkillState;
	
	// NormalAttack
	bool m_bHasStartedComboKeyInputCheck;
	int32 m_CurNormalAttackSection;
	int32 m_MaxNormalAttackSection;
	bool m_bIsStrikeAttackActive;
	FTimerHandle m_ShiftDecisionTimerHandle;

	UPROPERTY(EditAnywhere, Category = "StrikeAttackDecision")
	float m_StrikeAttackDecisionTime;
	
	UPROPERTY(EditAnywhere, Category = "NormalAttack_OnGround")
	float m_NormalAttackOnGroundMoveDistance;
	
	// UpperAttack
	UPROPERTY(EditAnywhere, Category = "UpperAttack_GroundToAir")
	float m_UpperAttackGroundToAirJumpDistance;

	UPROPERTY(EditAnywhere, Category = "DashAttack_OnGround")
	float m_DashAttackOnGroundMoveDistance;
	
	// AirToGroundAttack
	FTimerHandle m_EarthStrikeTimer;
	
	UPROPERTY(EditAnywhere, Category = "EarthStrike | Effect")
	TObjectPtr<UNiagaraSystem> m_EarthStrikeEffect;
	
	UPROPERTY(EditAnywhere, Category = "EarthStrike | CameraShake")
	TSubclassOf<UMatineeCameraShake> m_EarthStrikeCameraShake;

	UPROPERTY(EditAnywhere, Category = "EarthStrike | CameraShake")
	TObjectPtr<USoundWave> m_EarthStrikeSound;
	
	// Dodge
	UPROPERTY(EditAnywhere, Category = "Dodge_OnGround")
	float m_DodgeOnGroundMoveDistance;

	float m_GravityScaleInAir;
};

