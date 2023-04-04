// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "MainPlayer.generated.h"

enum class EMainPlayerStates : uint8;

UCLASS()
class HYOBINSPROJECT_API AMainPlayer : public ACharacterBase
{
	GENERATED_BODY()

public:
	AMainPlayer();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	// AxisMappings
	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);

	// ActionMappings
	void TriggerPressedShift();
	void TriggerReleasedShift();
	void TriggerPressedLeftMouseButton();
	void TriggerReleasedLeftMouseButton();

	void CheckNormalAttackCollision();


	// Get  자동 inline 처리된다.
	
	bool GetIsPressingShift() { return m_bIsPressingShift; }
	bool GetIsCombat() { return m_bIsCombated; }
	bool GetIsRunning() { return m_bIsRunning; }
	bool GetIsAttacking() { return m_bIsAttacking; }
	bool GetIsHit() { return m_bIsHit; }

	// Set


protected:
	virtual void BeginPlay() override;

private:
	void initAssets();
	
	void updateState();
	void normalComboAttack();
	void updateNormalAttackStateOnStart();
	void updateNormalAttackStateOnEnd();

	UFUNCTION()
		void OnNormalAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnCalledCheckNextAttackNotify();

	void printLog();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* m_SpringArm; // 이 컴포넌트로 등록된 자식 컴포넌트를
												// 자신과의 지정된 거리 안에 유지되도록 처리한다.

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* m_TargetCamera;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* m_SwordCollider;

private:
	TWeakObjectPtr<class UMainPlayerAnim> m_AnimInstance;

	float m_ArmLengthTo;
	FRotator m_ArmRotationTo;
	float m_ArmLengthSpeed;
	float m_ArmRotationSpeed;
	float m_MovdDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;
	bool m_bIsPressingShift;
	bool m_bIsCombated;
	bool m_bIsHit;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool m_bCanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool m_bIsInputOnNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 m_CurNormalAttackCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 m_NormalAttackMaxCombo;

	float m_NormalAttackRange;
	float m_NormalAttackRadius;

};
