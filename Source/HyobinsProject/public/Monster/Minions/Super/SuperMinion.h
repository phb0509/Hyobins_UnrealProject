// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SuperMinion.generated.h"

class USuperMinionAnim;
class ASuperMinionAIController;
class UAIPerceptionComponent;

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
	
	virtual void OnCalledTimer_EndedOnHitKnockback() override;

	UFUNCTION()
	void SkillMontageStarted(UAnimMontage* Montage);
	
	UFUNCTION()
	void SkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	
	// Get
	FORCEINLINE ENormalMinionStates GetState() const { return m_CurState; }

	// Set
	void SetState(ENormalMinionStates state);

protected:
	virtual void ExecOnHitEvent(ACharacterBase* instigator) override;
	virtual void Die() override;
	virtual void ExecEvent_EndedDeathMontage() override;
	void OnCalledTimer_EndedDeathEvent();

	virtual void SetCommonState(const int32 commonStateIndex) override;

	// IPoolableActor VirtualFunction 
	virtual void Activate() override;
	virtual void DeActivate() override;
	
private:
	void initAssets();
	void updateState();
	void bindFuncOnMontageEvent();

public:
	static const FName HitColliderName;
	
private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* m_HitCollider;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	ENormalMinionStates m_CurState;
	
	TWeakObjectPtr<ASuperMinionAIController> m_AIController;

	TWeakObjectPtr<USuperMinionAnim> m_AnimInstance;
	
	static int32 TagCount;
	static const FName HitMontages[8]; // 8����
	static const FName DeathMontages[8];
};
