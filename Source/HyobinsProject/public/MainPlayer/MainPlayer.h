// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CharacterBase.h"
#include "MainPlayer.generated.h"

class UMainPlayerSkillComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;

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
	
	void RotateActorToKeyInputDirection();
	void RotateActorToControllerYaw(); // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.
	
	// AxisMappings
	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);

	// ActionMappings
	void TriggerPressedLeftShift();
	void TriggerReleasedLeftShift();
	void TriggerPressedLeftMouseButton();
	void TriggerReleasedLeftMouseButton();
	void TriggerPressedRightMouseButton();
	void TriggerReleasedRightMouseButton();
	void TriggerPressedSpaceBar();
	void TriggerPressedLeftCtrl();
	void TriggerReleasedLeftCtrl();
	void TriggerPressedQ();
	
	FORCEINLINE void UpdateTempInput() { m_TempInputHorizontalForDodge = m_CurInputHorizontal; m_TempInputVerticalForDodge = m_CurInputVertical; }
	
	// Get
	FORCEINLINE UMainPlayerSkillComponent* GetSkillComponent() const { return m_SkillComponent; }
	FORCEINLINE int32 GetCurInputVertical() const { return m_CurInputVertical; }
	FORCEINLINE int32 GetCurInputHorizontal() const { return m_CurInputHorizontal; }
	FORCEINLINE bool GetIsPressedKey(const FName& key) const { return m_PressedKeyInfo[key]; }

	
	// Overlap시 호출시킬 바인딩 할 함수. 매개변수는 고정되어 있으므로, 바뀌면 안된다.
	UFUNCTION()
	void OnCalled_Overlap_SwordCollider(UPrimitiveComponent* HitComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCalled_Overlap_ShieldForAttackCollider(UPrimitiveComponent* HitComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCalled_Overlap_ShieldForDefendCollider(UPrimitiveComponent* HitComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
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
	UMainPlayerSkillComponent* m_SkillComponent;
	
	UPROPERTY(EditAnywhere) // 충돌체들은 실시간으로 크기보정할 일이 많을 수 있기 때문에 EditAnywhere로 지정
	UCapsuleComponent* m_SwordCollider;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_ShieldForAttackCollider;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_ShieldForDefendCollider;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	USpringArmComponent* m_SpringArm; // 이 컴포넌트로 등록된 자식 컴포넌트를
									  // 자신과의 지정된 거리 안에 유지되도록 처리한다.
	
	UPROPERTY(EditAnywhere, Category = Camera)
	UCameraComponent* m_TargetCamera;

	TMap<FName, bool> m_PressedKeyInfo;
	
	float m_MoveDeltaSecondsOffset;
	float m_RotationDeltaSecondsOffset;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool m_bIsTargeting;
	

	// KeyInput
	
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
