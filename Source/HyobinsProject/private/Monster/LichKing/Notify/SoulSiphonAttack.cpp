// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/Notify/SoulSiphonAttack.h"
#include "Monster/LichKing/LichKing.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystems/BattleManager.h"

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

		UBattleManager* battleManager = owner->GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
		check(battleManager != nullptr);
		
		TArray<FHitResult> hitResults;
		FCollisionShape sphereShape = FCollisionShape::MakeSphere(m_SphereCollisionRadius);
		FCollisionQueryParams params;
		FCollisionObjectQueryParams objectParams = battleManager->MakeCollisionObjectParams(owner);
	
		bool bHit = owner->GetWorld()->SweepMultiByObjectType(
			hitResults,
			startLocation,
			startLocation,
			FQuat::Identity,
			objectParams, 
			sphereShape,
			params
		);
		
		if (bHit)
		{
			for (const FHitResult& hitResult : hitResults)
			{
				ACharacterBase* hitActor = Cast<ACharacterBase>(hitResult.GetActor());
				const FName attackName = TEXT("SoulSiphon");
				
				if (hitActor != nullptr && !owner->HasContainHitActor(attackName, hitActor))
				{
					owner->AddHitActorsByMe(attackName, hitActor);
					battleManager->Attack(owner, attackName, hitActor, hitResult.Location);
				}
			}
		}
		
		playAttackEffect(owner);
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
