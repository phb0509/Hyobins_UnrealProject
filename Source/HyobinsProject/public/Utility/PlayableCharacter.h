// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "PlayableCharacter.generated.h"

struct FInputActionValue;
class UMainPlayerSkillComponent;
class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;
class UInputMappingContext;
class UInputAction;

DECLARE_MULTICAST_DELEGATE(FOnChangeInputMappingContextDelegate);

UCLASS(Abstract)
class HYOBINSPROJECT_API APlayableCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayableCharacter();

	virtual void BeginPlay() override;
	
	void RotateActorToKeyInputDirection();
	void RotateActorToControllerYaw(); // ������ z��ȸ������ ��Ʈ�ѷ��� z��ȸ�������� ����.
	
	// Get
	FORCEINLINE int32 GetCurInputVertical() const { return m_CurInputVertical; }
	FORCEINLINE int32 GetCurInputHorizontal() const { return m_CurInputHorizontal; }
	FORCEINLINE int32 GetDirectionIndexFromKeyInput() const { return DirectionIndex[m_CurInputVertical + 1][m_CurInputHorizontal + 1]; }
	FORCEINLINE TWeakObjectPtr<AActor> GetCurTarget() const { return m_CurTarget; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return m_MotionWarpingComponent; }
	FName GetHighestPriorityInputMappingContext() const;

	FVector GetForwardVectorFromControllerYaw() const;
	FVector GetRightVectorFromControllerYaw() const;
	FVector GetControllerKeyInputDirectionVector(const int32 keyInputDirection) const;
	int32 GetLocalDirection(const FVector& otherDirectionVector) const;

	
	// Set
	void SetCurTarget(AActor* target) { m_CurTarget = target; }

public:
	static const int32 DirectionIndex[3][3];
	FOnChangeInputMappingContextDelegate OnChangeInputMappingContext;

private:
	void initAssets();




	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionWarpingComponent> m_MotionWarpingComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> m_SpringArm; // �� ������Ʈ�� ��ϵ� �ڽ� ������Ʈ��
												 // �ڽŰ��� ������ �Ÿ� �ȿ� �����ǵ��� ó���Ѵ�.
	
	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> m_TargetCamera;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_CurInputHorizontal;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_CurInputVertical;

	TWeakObjectPtr<AActor> m_CurTarget;
	TMap<FName, int32> m_CurActiveMappingContexts;
	
	float m_MoveDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> m_OnHitCameraShake;
    
	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> m_AttackCameraShake;
};
