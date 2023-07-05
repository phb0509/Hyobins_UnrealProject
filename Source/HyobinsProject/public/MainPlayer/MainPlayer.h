// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "MainPlayer.generated.h"

enum class EMainPlayerStates : uint8;

UCLASS()
class HYOBINSPROJECT_API AMainPlayer final: public ACharacterBase
{
	GENERATED_BODY()

public:
	AMainPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
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
	void TriggerPressedSpaceBar();

protected:
	virtual void BeginPlay() override;

private:
	void initAssets();
	
	void updateState();
	void normalComboAttack();
	void updateNormalAttackStateOnStart();
	void rotateUsingControllerYawAndInput();
	void setRotationToControllerYaw();

	UFUNCTION()
		void checkOverlapSwordCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void checkOverlapShieldCollisionForAttack(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void checkOverlapShieldCollisionForShield(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void onCalledNotify_NormalAttackHitCheck();
	void onCalledNotify_NormalAttackNextCheck();
	void onCalledNotify_EndedNormalAttack();
	void onCalledNotify_EndedDodgeMove();
	

	void printLog();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* m_SpringArm; // �� ������Ʈ�� ��ϵ� �ڽ� ������Ʈ��
												// �ڽŰ��� ������ �Ÿ� �ȿ� �����ǵ��� ó���Ѵ�.

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* m_TargetCamera;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* m_SwordCollider;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UBoxComponent* m_ShieldColliderForAttack;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UBoxComponent* m_ShieldColliderForShield;

private:
	TWeakObjectPtr<class UMainPlayerAnim> m_AnimInstance;

	float m_ArmLengthTo;
	FRotator m_ArmRotationTo;
	float m_ArmLengthSpeed;
	float m_ArmRotationSpeed;

	float m_MovdDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsCombated;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsHit;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bIsPressingShift;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsDodgeMoving;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_CurInputHorizontal;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_CurInputVertical;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_TempInputHorizontalForDodge;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_TempInputVerticalForDodge;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_TempIsAttacking;

	bool m_bCanNextCombo;
	bool m_bIsInputOnNextCombo;
	int32 m_CurNormalAttackCombo;
	int32 m_NormalAttackMaxCombo;

};
