// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase/CharacterBase.h"
#include "PlayableCharacter.generated.h"

class USkillComponent;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;
class UInputMappingContext;
class UInputAction;


USTRUCT(Atomic)
struct FInputConfig
{
	GENERATED_USTRUCT_BODY()

public:
	FName inputMappingContextName;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> inputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TMap<FName, TObjectPtr<UInputAction>> inputActions;
};


DECLARE_MULTICAST_DELEGATE(FOnChangeInputMappingContextDelegate);

UCLASS(Abstract)
class HYOBINSPROJECT_API APlayableCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayableCharacter();

	virtual void BeginPlay() override;

	void RotateActorToKeyInputDirection();
	void RotateActorToControllerYaw(); // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.

	UFUNCTION(BlueprintCallable, Category = "InputMappingContext")
	void AddInputMappingContext(const FName& inputMappingContextName);

	UFUNCTION(BlueprintCallable, Category = "InputMappingContext")
	void RemoveInputMappingContext(const FName& inputMappingContextName);

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsMoveKeyPressed() const { return m_CurInputHorizontal || m_CurInputVertical; }

	// Get
	FORCEINLINE int32 GetCurInputVertical() const { return m_CurInputVertical; }
	FORCEINLINE int32 GetCurInputHorizontal() const { return m_CurInputHorizontal; }

	FORCEINLINE int32 GetDirectionIndexFromKeyInput() const
	{
		return DirectionIndex[m_CurInputVertical + 1][m_CurInputHorizontal + 1];
	}

	FORCEINLINE TWeakObjectPtr<AActor> GetCurTarget() const { return m_CurTarget; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return m_MotionWarpingComponent; }
	FORCEINLINE USkillComponent* GetSkillComponent() const { return m_SkillComponent; }
	FName GetHighestPriorityInputMappingContext() const;

	FVector GetForwardVectorFromControllerYaw() const;
	FVector GetRightVectorFromControllerYaw() const;
	FVector GetWorldKeyInputDirection(const int32 keyInputDirection) const;
	int32 GetLocalDirectionIndex(const FVector& worldDirection) const;
	int32 GetLocalDirectionUsingInverseMatrix(const FVector& worldDirection) const;


	// Set
	void SetCurTarget(AActor* target) { m_CurTarget = target; }

	// Input

	// AxisMappings
	void Move(const FInputActionValue& value); // WSAD
	void Look(const FInputActionValue& value); // Mouse X,Y

	void InitArrowKeys()
	{
		m_CurInputHorizontal = 0;
		m_CurInputVertical = 0;
	}

	// ActionMappings
	void Run();
	void StopRun();
	void ToggleTargetMode();


public:
	static const int32 DirectionIndex[3][3];
	FOnChangeInputMappingContextDelegate OnChangeInputMappingContext;

private:
	void initAssets();
	void initInputConfigs();
	AActor* findNearestTarget();
	void lockOnToTarget(AActor* target);
	void updateCameraRotation();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "InputMappingContext")
	TMap<FName, FInputConfig> m_InputMappingConfigs;
	TMap<FName, int32> m_CurActiveMappingContexts;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkillComponent> m_SkillComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionWarpingComponent> m_MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> m_SpringArm; // 이 컴포넌트로 등록된 자식 컴포넌트를
												 // 자신과의 지정된 거리 안에 유지되도록 처리한다.

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> m_TargetCamera;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	int32 m_CurInputHorizontal;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	int32 m_CurInputVertical;

	TWeakObjectPtr<AActor> m_CurTarget;

	float m_MoveDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> m_OnHitCameraShake;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> m_AttackCameraShake;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	bool m_bIsPressedShift;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowProtectedAccess = true))
	bool m_bIsLockOn;
	
	FTimerHandle m_LockOnTimer;

	TWeakObjectPtr<AActor> m_CurLockOnTarget;

	FRotator m_InitialControlRotation;
	float m_LockOnElapsed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn", Meta = (AllowPrivateAccess = true))
	float m_LockOnRotaionDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn", Meta = (AllowPrivateAccess = true))
	float m_LockOnCameraPitch;
};
