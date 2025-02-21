// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SoulSiphonAttack.generated.h"

class ACharacterBase;

UCLASS()
class HYOBINSPROJECT_API USoulSiphonAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	USoulSiphonAttack();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void playAttackEffect(ACharacterBase* owner) const;
	void playHitEffect(AActor* target) const;
	
private:
	UPROPERTY(EditAnywhere)
	float m_SphereCollisionRadius;

	UPROPERTY(EditAnywhere)
	float m_SphereCollisionSpawnDistanceFromOwner;
	
	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<UParticleSystem> m_HitParticle;

	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<UParticleSystem> m_AttackParticle;
	
};
