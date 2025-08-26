// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_Check_Collision.generated.h"

class ACharacterBase;
class UDataManager;
class USoundWave;
class UParticleSystem;
class UCameraShakeBase;


UCLASS()
class HYOBINSPROJECT_API UNotifyState_Check_Collision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UNotifyState_Check_Collision();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	void playHitEffect(const FVector& overlapLocation);


private:
	TWeakObjectPtr<ACharacterBase> m_Owner;
	
	UPROPERTY(EditAnywhere)
	FName m_ColliderName;

	UPROPERTY(EditAnywhere)
	FName m_AttackName;

	UPROPERTY(EditAnywhere, Category = "OnHit Effect")
	TObjectPtr<USoundWave> m_OnHitSound;

	UPROPERTY(EditAnywhere, Category = "OnHit Effect")
	TObjectPtr<UParticleSystem> m_OnHitParticle;

	UPROPERTY(EditAnywhere, Category = "OnHit Effect")
	TSubclassOf<UCameraShakeBase> m_OnHitCameraShake;

	UPROPERTY(EditAnywhere, Category = "OnHit Effect")
	float m_MontagePlayRate;

	UPROPERTY(EditAnywhere, Category = "OnHit Effect")
	float m_MontageDelayTime;

	float m_MontageDefaultPlayRate;

	TWeakObjectPtr<UShapeComponent> m_WeaponCollider;
	FVector m_PrevColliderLocation;
};



