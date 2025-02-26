// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/Notify/SoulSiphonAttack.h"
#include "Monster/LichKing/LichKing.h"
#include "Interfaces/Damageable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

USoulSiphonAttack::USoulSiphonAttack() :
	m_SphereCollisionRadius(300.0f),
	m_SphereCollisionSpawnDistanceFromOwner(50.0f),
	m_HitParticle(nullptr),
	m_AttackParticle(nullptr)
{
}

void USoulSiphonAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacterBase* owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	
	if (owner != nullptr)
	{
		FVector startLocation = owner->GetActorLocation();
		startLocation += owner->GetActorForwardVector() * m_SphereCollisionSpawnDistanceFromOwner;

		const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {
			UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5)
		};
		const TArray<AActor*> IgnoreActors = {owner};
		TArray<AActor*> overlappedActors;

		UKismetSystemLibrary::SphereOverlapActors(
			MeshComp->GetWorld(),
			startLocation,
			m_SphereCollisionRadius, // 구체 반지름
			ObjectTypes,
			nullptr,
			IgnoreActors,
			overlappedActors);

		playAttackEffect(owner);
		
		if (overlappedActors.Num() > 0)
		{
			for (AActor* overlappedEnemy : overlappedActors)
			{
				if (overlappedEnemy != nullptr)
				{
					owner->Attack(TEXT("SoulSiphon"), overlappedEnemy);
				}
				
				playHitEffect(overlappedEnemy);
			}
		}
	}
}

void USoulSiphonAttack::playAttackEffect(ACharacterBase* owner) const
{
	if (m_AttackParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
		   owner->GetWorld(),
		   m_AttackParticle, 
		   owner->GetActorLocation(), 
		   owner->GetActorRotation()  
	   );
	}
}

void USoulSiphonAttack::playHitEffect(AActor* target) const
{
	if (m_HitParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
		   target->GetWorld(),
		   m_HitParticle, 
		   target->GetActorLocation(), 
		   FRotator(0.0f)  
	   );
	}
}
