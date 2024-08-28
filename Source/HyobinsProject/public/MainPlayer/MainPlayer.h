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
//class UInputConfigData;

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

	// ActionMappings

	void NormalAttack();
	void Dodge();
	void UpperAttack();
	void Run();
	void StopRun();

	void RotateActorToKeyInputDirection();
	void RotateActorToControllerYaw(); // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.
	
	FORCEINLINE void UpdateTempInput() { m_TempInputHorizontalForDodge = m_CurInputHorizontal; m_TempInputVerticalForDodge = m_CurInputVertical; }
	
	// Get
	FORCEINLINE UMainPlayerSkillComponent* GetSkillComponent() const { return m_SkillComponent; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return m_MotionWarpingComponent; }
	FORCEINLINE int32 GetCurInputVertical() const { return m_CurInputVertical; }
	FORCEINLINE int32 GetCurInputHorizontal() const { return m_CurInputHorizontal; }
	FORCEINLINE bool GetIsPressedKey(const FName& key) const { return m_PressedKeyInfo[key]; }

	

	
private:
	void initAssets();
	void updateState();
	void printLog() const;
	
public:
	static const FName SwordColliderName;
	static const FName ShieldForAttackColliderName;
	static const FName ShieldForDefendColliderName;
	
private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UMainPlayerSkillComponent> m_SkillComponent;
	
	UPROPERTY(EditAnywhere) // 충돌체들은 실시간으로 크기보정할 일이 많을 수 있기 때문에 EditAnywhere로 지정
	TObjectPtr<UCapsuleComponent> m_SwordCollider;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_ShieldForAttackCollider;
    
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_ShieldForDefendCollider;
	
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
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsTargeting;
	

	// KeyInput
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> m_InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput", Meta = (AllowPrivateAccess = true))
	TMap<FName, TObjectPtr<UInputAction>> m_InputActions;

	
	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	bool m_bIsPressedShift;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_CurInputHorizontal;

	UPROPERTY(BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_CurInputVertical;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_TempInputHorizontalForDodge;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = true))
	int32 m_TempInputVerticalForDodge;

	
};
