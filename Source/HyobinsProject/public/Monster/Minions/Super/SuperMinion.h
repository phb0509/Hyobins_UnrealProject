// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SuperMinion.generated.h"

class USuperMinionAnim;
class ASuperMinionAIController;
class UAIPerceptionComponent;
class UBoxComponent;

enum class EMonsterCommonStates : uint8;
enum class ENormalMinionStates : uint8;

UCLASS()
class HYOBINSPROJECT_API ASuperMinion final: public AMonster
{
	GENERATED_BODY()

public:
	ASuperMinion();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCalled_NormalAttack_End();
	
	virtual void OnCalledTimer_Knockback_End() override;

	UFUNCTION()
	void SkillMontageStarted(UAnimMontage* Montage);
	
	UFUNCTION()
	void SkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	
	// Get
	FORCEINLINE ENormalMinionStates GetState() const { return m_CurState; }

	// Set
	void SetState(ENormalMinionStates state);

protected:
	virtual void ExecEvent_Knockback(ACharacterBase* instigator) override;
	virtual void ExecEvent_Groggy(ACharacterBase* instigator) override;
	virtual void Die() override;
	virtual void ExecEvent_EndedDeathMontage() override;
	void onCalledTimer_EndedDeathEvent();

	virtual void SetCommonState(const int32 commonStateIndex) override;

	// IPoolableActor VirtualFunction 
	virtual void Activate() override;
	virtual void DeActivate() override;
	
private:
	void initAssets();
	void updateState();

	
private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* m_HitCollider;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_LeftSwordCollider;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_RightSwordCollider;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	ENormalMinionStates m_CurState;
	
	TWeakObjectPtr<ASuperMinionAIController> m_AIController;
	TWeakObjectPtr<USuperMinionAnim> m_AnimInstance;
	
	static int32 TagCount;
	static const FName HitColliderName;
	static const FName LeftSwordColliderName;
	static const FName RightSwordColliderName;
};
