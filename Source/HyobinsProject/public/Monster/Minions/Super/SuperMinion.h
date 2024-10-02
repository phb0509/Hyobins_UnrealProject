// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SuperMinion.generated.h"

class USuperMinionAnim;
class ASuperMinionAIController;
enum class ESuperMinionFSMStates : uint8;

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
	
	
	// Get
	FORCEINLINE ESuperMinionFSMStates GetState() const { return m_CurFSMState; }

	// Set
	void SetFSMState(ESuperMinionFSMStates state);

protected:
	virtual void ExecEvent_TakeKnockbackAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) override;
	virtual void OnCalledTimer_KnockbackOnStanding_End() override;

	virtual void ExecEvent_TakeAirborneAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) override;
	virtual void ExecEvent_Down_WhenOnGround() override;

	virtual void ExecEvent_TakeDownAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) override;
	virtual void OnCalledTimer_Down_End() override;
	
	virtual void Die() override;
	virtual void ExecEvent_EndedDeathMontage() override;
	virtual void OnCalledTimer_EndedDeathMontage() override {};
	
	UFUNCTION()
	void OnCalledTimelineEvent_Loop_AfterDeath(float curveValue);

	UFUNCTION()
	void OnCalledTimelineEvent_End_AfterDeath();

	void DisableMovementForDuration(float duration) const;
	void CallTimer_ExecDownEvent_WhenOnGround();
	
	// IPoolableActor VirtualFunction 
	virtual void Activate() override;
	virtual void DeActivate() override;

	
private:
	void initAssets();
	void bindFuncOnMontagEvent();
	

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> m_HitCollider;
	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_LeftSwordCollider;
		
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_RightSwordCollider;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	ESuperMinionFSMStates m_CurFSMState;
	
	TWeakObjectPtr<ASuperMinionAIController> m_AIController;
	TWeakObjectPtr<USuperMinionAnim> m_AnimInstance;
	
	static int32 TagCount;
	static const FName HitColliderName;
	static const FName LeftSwordColliderName;
	static const FName RightSwordColliderName;
	static const FName KnockbackMontageNames[4];
	static const FName DeathMontageNames[2];
	
};
