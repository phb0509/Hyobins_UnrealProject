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
	virtual void PossessedBy(AController* newController) override;
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

	// Get  자동 inline 처리된다.
	
	bool GetIsPressingShift() { return m_bIsPressingShift; }
	bool GetIsCombat() { return m_bIsCombated; }
	bool GetIsRunning() { return m_bIsRunning; }
	bool GetIsAttacking() { return m_bIsNormalAttacking; }
	bool GetIsHit() { return m_bIsHit; }

	// Set


protected:
	virtual void BeginPlay() override;

private:
	void initComponents();
	void initCollisions();
	void initSpringArm();
	void initTargetCamera();
	void initAttackInformations();
	void checkIsValidComponants();
	void updateState();
	void normalAttack();
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
		class UCapsuleComponent* m_SwordCollision;

private:
	float m_ArmLengthTo;
	FRotator m_ArmRotationTo;
	float m_ArmLengthSpeed;
	float m_ArmRotationSpeed;
	float m_RunSpeed;
	float m_MovdDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;
	bool m_bIsPressingShift;
	bool m_bIsCombated;
	bool m_bIsRunning;
	bool m_bIsHit;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool m_bIsNormalAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool m_bCanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool m_bIsInputOnNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 m_CurNormalAttackCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 m_NormalAttackMaxCombo;

	TWeakObjectPtr<class UMainPlayerAnim> m_ABPAnimInstance;
};
