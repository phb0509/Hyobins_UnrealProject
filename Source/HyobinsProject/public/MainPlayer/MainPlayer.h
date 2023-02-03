// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/CustomStructs.h"
#include "MainPlayer.generated.h"

enum class EMainPlayerStates : uint8;

UCLASS()
class HYOBINSPROJECT_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	// AxisMappings
	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);

	// ActionMappings
	void TriggerPressedShift();
	void TriggerReleasedShift();
	void TriggerPressedMoveWSAD();
	void TriggerReleasedMoveWSAD();
	void TriggerPressedLeftMouseButton();
	void TriggerReleasedLeftMouseButton();

	//UFUNCTION(BlueprintCallable, Category = "FSM")
	//	EMainPlayerStates GetFSMState() { return m_CurState; }

	// Get 자동 inline 처리.
	bool GetIsIdle() { return m_bIsIdle; }
	bool GetIsPressingShift() { return m_bIsPressingShift; }
	bool GetIsCombat() { return m_bIsCombated; }
	bool GetIsWalking() { return m_bIsWalking; }
	bool GetIsRunning() { return m_bIsRunning; }
	bool GetIsInAir() { return m_bIsInAir; }
	bool GetIsAttacking() { return m_bIsAttacking; }
	bool GetIsHit() { return m_bIsHit; }

	// Set


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void initComponents();
	void initSwordCollision();
	void initSpringArm();
	void initTargetCamera();
	void checkIsValidComponants();
	void updateState();
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
	float m_CurSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_MovdDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;
	//EMainPlayerStates m_CurState;
	bool m_bIsIdle;
	bool m_bIsPressingShift;
	bool m_bIsCombated;
	bool m_bIsWalking;
	bool m_bIsRunning;
	bool m_bIsInAir;	
	bool m_bIsAttacking;
	bool m_bIsHit;
};
