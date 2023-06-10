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
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//virtual void Jump() override;

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

	void OnCalledNotify_NormalAttackHitCheck();


	// Get  �ڵ� inline ó���ȴ�.
	
	bool GetIsPressingShift() { return m_bIsPressingShift; }
	bool GetIsCombat() { return m_bIsCombated; }
	bool GetIsHit() { return m_bIsHit; }

	// Set


protected:
	virtual void BeginPlay() override;

private:
	void initAssets();
	
	void updateState();
	void normalComboAttack();
	void updateNormalAttackStateOnStart();
	void checkNormalAttackCollisionBySweep(); // ���� ����.
	
	UFUNCTION()
		void OnNormalAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void checkOverlapSwordCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void checkOverlapShieldCollisionForAttack(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void checkOverlapShieldCollisionForShield(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void OnCalledNotify_NormalAttackNextCheck();

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


};
