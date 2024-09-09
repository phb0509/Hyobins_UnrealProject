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
	
	void NormalAttack();
	void NormalAttack_OnGround();
	void NormalAttack_InAir();
	void UpperAttack();
	void ExtendShiftDecisionTime(); // 강공격키판정을 좀 더 여유롭게하기위한 타이머호출 함수.
	
	void AirToGroundAttack_InAir();
	void AirToGroundAttack_OnGround();
	void AirToGroundAttack_CollisionCheck();
	void AirToGroundAttack_PlayEffect();
	
	void Dodge();

	
	//NormalAttack
	UFUNCTION()
	void OnCalledNotify_NormalAttack_Start_EachSection();

	
	FORCEINLINE EMainPlayerSkillStates GetState() const { return m_CurSkillState; }
	FORCEINLINE void SetHasStartedComboKeyInputCheck(bool bHasStartedKeyInputCheck) { m_bHasStartedComboKeyInputCheck = bHasStartedKeyInputCheck;}
	FORCEINLINE void SetState(EMainPlayerSkillStates state) { m_CurSkillState = state; }
	
	UFUNCTION()
	FORCEINLINE void SetIdle(UAnimMontage* Montage, bool bInterrupted);
	
	
private:
	void bindFuncOnMontageEvent();
	void linqNextNormalAttackCombo();
	void linqNextNormalAttackInAirCombo();
	

private:
	TWeakObjectPtr<AMainPlayer> m_Owner;
	TWeakObjectPtr<UMainPlayerAnim> m_OwnerAnimInstance;
	EMainPlayerSkillStates m_CurSkillState;
	
	// NormalAttack
	bool m_bHasStartedComboKeyInputCheck;
	int32 m_CurNormalAttackSection;
	int32 m_MaxNormalAttackSection;
	bool m_bHasleftShiftDecision;
	FTimerHandle m_ShiftDecisionTimerHandle;

	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	float m_NormalAttackMoveDistance;
	
	// UpperAttack
	UPROPERTY(EditAnywhere, Category = "UpperAttackToAir")
	float m_UpperAttackToAirJumpDistance;

	// AirToGroundAttack
	FTimerHandle m_AirToGroundAttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "AirToGroundAttack | Effect")
	TObjectPtr<UNiagaraSystem> m_AirToGroundAttackEffect;
	
	UPROPERTY(EditAnywhere, Category = "AirToGroundAttack | CameraShake")
	TSubclassOf<UMatineeCameraShake> m_AirToGroundAttackCameraShake;

	// Dodge
	float m_DodgeMoveDistance;

	//Effect
	
};

