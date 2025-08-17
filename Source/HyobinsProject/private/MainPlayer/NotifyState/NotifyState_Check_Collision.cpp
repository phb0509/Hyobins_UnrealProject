// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Check_Collision.h"
#include "CharacterBase/CharacterBase.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayableCharacter/PlayableCharacter.h"


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
	
	if (m_Owner.IsValid())
	{
		m_WeaponCollider = m_Owner->GetCollider(m_ColliderName);

		if (m_WeaponCollider != nullptr)
		{
			m_PrevColliderLocation = m_WeaponCollider->GetComponentLocation();
		
			if (m_AttackName != TEXT("Empty"))
			{
				m_Owner->EmptyHitActorsByMe(m_AttackName);
			}
		}
	}
}

void UNotifyState_Check_Collision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (m_Owner.IsValid() && m_WeaponCollider != nullptr)
	{
		FVector curLocation = m_WeaponCollider->GetComponentLocation();
		
		TArray<FHitResult> hitResults;
		FCollisionQueryParams params;
		
		FCollisionShape collisionShape = makeCollisionShape();
		FCollisionObjectQueryParams objectParams = makeCollisionObjectParams();

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
				AActor* hitActor = hitResult.GetActor();
				if (IsValid(hitActor) && !m_Owner->HasContainHitActor(m_AttackName, hitActor))
				{
					m_Owner->AddHitActorsByMe(m_AttackName, hitActor);
					m_Owner->Attack(m_AttackName, hitActor, hitResult.Location);

					// 이펙트, 사운드, 카메라쉐이크 등
					playHitEffect(hitResult.ImpactPoint);
				}
			}
		}

		// 위치 갱신
		m_PrevColliderLocation = curLocation;
	}
}

void UNotifyState_Check_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
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
		m_Owner.Get()->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_OnHitCameraShake);
	}


	// StopMotion
	UAnimInstanceBase* animInstanceBase = m_Owner->GetAnimInstanceBase();
	UAnimMontage* curMontage = animInstanceBase->GetCurrentActiveMontage();

	if (curMontage != nullptr)
	{
		//const float curMontagePlayRate = animInstanceBase->Montage_GetPlayRate(curMontage);
	
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

FCollisionShape UNotifyState_Check_Collision::makeCollisionShape() const
{
	if (auto* Sphere = Cast<USphereComponent>(m_WeaponCollider))
	{
		return FCollisionShape::MakeSphere(Sphere->GetScaledSphereRadius());
	}

	if (auto* Box = Cast<UBoxComponent>(m_WeaponCollider))
	{
		return FCollisionShape::MakeBox(Box->GetScaledBoxExtent());
	}
	
	if (auto* Capsule = Cast<UCapsuleComponent>(m_WeaponCollider))
	{
		return FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());
	}

	return FCollisionShape::MakeSphere(10.0f);
}

FCollisionObjectQueryParams UNotifyState_Check_Collision::makeCollisionObjectParams() const
{
	FCollisionObjectQueryParams collisionObjectParams;
	
	if (APlayableCharacter* playableCharacter = Cast<APlayableCharacter>(m_Owner))
	{
		collisionObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	}
	else
	{
		collisionObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel5);
	}

	return collisionObjectParams;
}
