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
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void OnCalledNotify_EndedNormalAttack();
	virtual void OnCalledTimer_OnHit() override;

	UFUNCTION()
	void SkillMontageStarted(UAnimMontage* Montage);
	
	UFUNCTION()
	void SkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void TestPrint()
	{
		UE_LOG(LogTemp, Warning, TEXT("TestPrint!!!!!!!!!!!!!"));
	}
	
	
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

private:
	void initAssets();
	void updateState();
	

public:
	static int TagCount;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ENormalMinionStates m_CurState;
	
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<ASuperMinionAIController> m_AIController;

	TWeakObjectPtr<USuperMinionAnim> m_AnimInstance;
};
