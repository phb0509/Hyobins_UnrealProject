// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Check_Collision.h"
#include "CharacterBase/CharacterBase.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "SubSystems/BattleManager.h"


UNotifyState_Check_Collision::UNotifyState_Check_Collision() :
	m_ColliderName(""),
	m_AttackName(""),
	m_OnHitSound(nullptr),
	m_OnHitParticle(nullptr),
	m_OnHitCameraShake(nullptr),
	m_MontagePlayRate(0.1f),
	m_MontageDelayTime(0.1f),
	m_MontageDefaultPlayRate(1.0f),
	m_WeaponCollider(nullptr),
	m_PrevColliderLocation(FVector(0.0f, 0.0f, 0.0f))
{
}

void UNotifyState_Check_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	m_Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	
	if (m_Owner != nullptr)
	{
		m_WeaponCollider = m_Owner->GetCollider(m_ColliderName);
		check(m_WeaponCollider != nullptr);
		
		m_PrevColliderLocation = m_WeaponCollider->GetComponentLocation();

		if (m_AttackName != TEXT("Empty"))
		{
			m_Owner->EmptyHitActorsByMe(m_AttackName);
		}
	}
}

void UNotifyState_Check_Collision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (m_Owner.IsValid() && m_WeaponCollider.IsValid())
	{
		UBattleManager* battleManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
		check(battleManager != nullptr);
		
		FVector curLocation = m_WeaponCollider->GetComponentLocation();
		
		TArray<FHitResult> hitResults;
		FCollisionQueryParams params;
		
		FCollisionShape collisionShape = battleManager->MakeCollisionShape(m_WeaponCollider.Get());
		FCollisionObjectQueryParams objectParams = battleManager->MakeCollisionObjectParams(m_Owner.Get());

		bool bHit = m_Owner->GetWorld()->SweepMultiByObjectType(
			hitResults,
			m_PrevColliderLocation,
			curLocation,
			m_WeaponCollider->GetComponentQuat(),
			objectParams, 
			collisionShape,
			params
		);
		
		if (bHit)
		{
			for (const FHitResult& hitResult : hitResults)
			{
				ACharacterBase* hitActor = Cast<ACharacterBase>(hitResult.GetActor());
				
				if (hitActor != nullptr && !m_Owner->HasContainHitActor(m_AttackName, hitActor))
				{
					m_Owner->AddHitActorsByMe(m_AttackName, hitActor);
					battleManager->Attack(m_Owner.Get(), m_AttackName, hitActor, hitResult.Location);

					// 이펙트, 사운드, 카메라쉐이크 등
					playHitEffect(hitResult.ImpactPoint);
				}
			}
		}
		
		m_PrevColliderLocation = curLocation;
	}
}

void UNotifyState_Check_Collision::playHitEffect(const FVector& overlapLocation)
{
	// Sound
	if (m_OnHitSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(m_Owner.Get(), m_OnHitSound, 2.5f);
	}

	// Particle
	if (m_OnHitParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			m_Owner->GetWorld(),
			m_OnHitParticle,
			overlapLocation,
			FRotator::ZeroRotator,
			true, // bAutoDestroy
			EPSCPoolMethod::AutoRelease, // PoolingMethod
		true 
		);
	}

	// CameraShake
	if (m_OnHitCameraShake != nullptr)
	{
		m_Owner->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_OnHitCameraShake);
	}


	// StopMotion
	UAnimInstanceBase* animInstanceBase = m_Owner->GetAnimInstanceBase();
	
	if (animInstanceBase != nullptr)
	{
		UAnimMontage* curMontage = animInstanceBase->GetCurrentActiveMontage();

		if (curMontage != nullptr)
		{
			animInstanceBase->Montage_SetPlayRate(curMontage,m_MontagePlayRate);
		
			FTimerHandle timer;
			m_Owner->GetWorldTimerManager().SetTimer
			( 
				timer,
				[=]()
				{
					animInstanceBase->Montage_SetPlayRate(curMontage,1.0f);
				},
				m_MontageDelayTime,
				false 
			);
		}
	}
}
