// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "LichKing.generated.h"

class ALichKingAIController;
class ULichKingAnim;
class UMotionWarpingComponent;

UCLASS()
class HYOBINSPROJECT_API ALichKing : public AMonster
{
	GENERATED_BODY()

public:
	ALichKing();

	virtual void BeginPlay() override;

protected:
	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation*, AActor* instigator, const FVector& causerLocation) override;
	
	//virtual void OnStaminaIsZero() override;

	virtual void ExecEvent_EndedDeathMontage() override;

	virtual void Die() override;
	
private:
	void initAssets();
	void printLog();
	

private:
	TWeakObjectPtr<ALichKingAIController> m_AIController;
	TWeakObjectPtr<ULichKingAnim> m_AnimInstance;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_HammerCollider;
};
