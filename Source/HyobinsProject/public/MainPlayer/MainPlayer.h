// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "MainPlayer.generated.h"

struct FInputActionValue;
class UMainPlayerSkillComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UMotionWarpingComponent;
struct FInputActionValue;
class AMainPlayer;
class UInputMappingContext;
class UInputAction;


enum class EMainPlayerStates : uint8;

UCLASS()
class HYOBINSPROJECT_API AMainPlayer final: public ACharacterBase
{
	GENERATED_BODY()

public:
	AMainPlayer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// AxisMappings
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void InitArrowKeys();
	
	// ActionMappings
	void Run() const;
	void StopRun() const;
	void Check_IsPressed_LeftShift();
	void Check_IsReleased_LeftShift();

	UFUNCTION(BlueprintCallable, Category = "InputMappingContext")
	void AddInputContextMappingInAir();

	UFUNCTION(BlueprintCallable, Category = "InputMappingContext")
	void RemoveInputContextMappingInAir();
	
	void RotateActorToKeyInputDirection();
	void RotateActorToControllerYaw(); // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.
	
	// Get
	FORCEINLINE UMainPlayerSkillComponent* GetSkillComponent() const { return m_SkillComponent; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return m_MotionWarpingComponent; }
	FORCEINLINE int32 GetCurInputVertical() const { return m_CurInputVertical; }
	FORCEINLINE int32 GetCurInputHorizontal() const { return m_CurInputHorizontal; }
	FORCEINLINE bool GetIsPressedKey(const FName& key) const { return m_PressedKeyInfo[key]; }

	

private:
	void initAssets();
	void printLog() const;
	
public:
	static const FName SwordColliderName;
	static const FName ShieldForAttackColliderName;
	static const FName ShieldForDefendColliderName;
	
private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMainPlayerSkillComponent> m_SkillComponent;
	
	UPROPERTY(EditAnywhere) 
	TObjectPtr<UCapsuleComponent> m_SwordCollider;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_ShieldForAttackCollider;
    
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_ShieldForDefendCollider;

	UPROPERTY(EditAnywhere) 
	TObjectPtr<UCapsuleComponent> m_ShieldBottomCollider;
	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionWarpingComponent> m_MotionWarpingComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> m_SpringArm; // 이 컴포넌트로 등록된 자식 컴포넌트를
									  // 자신과의 지정된 거리 안에 유지되도록 처리한다.

	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> m_TargetCamera;
	
	TMap<FName, bool> m_PressedKeyInfo;
	
	float m_MoveDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;
	
	
	// KeyInput
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | OnGround", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> m_InputMappingContextOnGround;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | OnGround", Meta = (AllowPrivateAccess = true))
	TMap<FName, TObjectPtr<UInputAction>> m_InputActionsOnGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | InAir", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> m_InputMappingContextInAir;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput | InAir", Meta = (AllowPrivateAccess = true))
	TMap<FName, TObjectPtr<UInputAction>> m_InputActionsInAir;
	
	
	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bIsPressedShift;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_CurInputHorizontal;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_CurInputVertical;

	// UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	// int32 m_TempInputHorizontalForDodge;
	//
	// UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	// int32 m_TempInputVerticalForDodge;

	
};
