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
	void TriggerPressedLeftCtrl();
	void TriggerReleasedLeftCtrl();


protected:
	virtual void BeginPlay() override;

private:
	void initAssets();
	void updateState();
	void normalComboAttack();
	void updateNormalAttackStateOnStart();
	void rotateUsingControllerYawAndInput();
	void setRotationToControllerYaw(); // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.

	// Overlap시 호출시킬 바인딩 할 함수. 매개변수는 고정되어 있으므로, 바뀌면 안된다.
	UFUNCTION()
		void checkOverlapSwordCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void checkOverlapShieldCollisionForAttack(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void checkOverlapShieldCollisionForShield(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// UFUNCTION()
	// void onMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void onCalledNotify_NormalAttackHitCheck();
	void onCalledNotify_NormalAttackNextCheck();
	void onCalledNotify_EndedNormalAttack();
	void onCalledNotify_EndedDodgeMove();
	void printLog();

	
private:
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	class USpringArmComponent* m_SpringArm; // 이 컴포넌트로 등록된 자식 컴포넌트를
											// 자신과의 지정된 거리 안에 유지되도록 처리한다.
	
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* m_TargetCamera;

	UPROPERTY(EditAnywhere, Category = Collision) // 충돌체들은 실시간으로 크기보정할 일이 많을 수 있기 때문에 EditAnywhere로 지정
	UCapsuleComponent* m_SwordCollider;

	UPROPERTY(EditAnywhere, Category = Collision)
	class UBoxComponent* m_ShieldColliderForAttack;

	UPROPERTY(EditAnywhere, Category = Collision)
	class UBoxComponent* m_ShieldColliderForShield;

	TMap<FName, FAttackInfoStruct> m_AttackInformations; // 임시. SkillComponent로 옮길것.

	UPROPERTY(VisibleDefaultsOnly, Category = SkillComponent, Meta = (AllowPrivateAccess = true))
	class UMainPlayerSkillComponent* m_SkillComponent;

	TWeakObjectPtr<class UMainPlayerAnim> m_AnimInstance;

	float m_ArmLengthTo;
	//FRotator m_ArmRotationTo;
	float m_ArmLengthSpeed;
	//float m_ArmRotationSpeed;

	float m_MoveDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsDodgeMoving;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsCombated;

	
	UPROPERTY(BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsHit;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bIsPressingShift;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_CurInputHorizontal;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_CurInputVertical;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_TempInputHorizontalForDodge;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int m_TempInputVerticalForDodge;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bTempIsAttacking;

	// NormalAttack관련.. 옮길예정.
	bool m_bCanNextCombo;
	bool m_bIsInputOnNextCombo;
	int32 m_CurNormalAttackCombo;
	int32 m_NormalAttackMaxCombo;
};
