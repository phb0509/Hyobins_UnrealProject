// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Check_Collision.h"
#include "CharacterBase/CharacterBase.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"


UNotifyState_Check_Collision::UNotifyState_Check_Collision() :
	m_HitSound(nullptr)
{
}

void UNotifyState_Check_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	m_Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	if (m_Owner.IsValid())
	{
		m_Owner->GetCollider(m_ColliderName)->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
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
	
	if (m_Owner.IsValid())
	{
		TArray<AActor*> overlappedActors;

		UShapeComponent* weaponCollider = m_Owner->GetCollider(m_ColliderName);

		if (IsValid(weaponCollider))
		{
			weaponCollider->GetOverlappingActors(overlappedActors);
		
			if (overlappedActors.Num() > 0)
			{
				for (AActor* overlappedActor : overlappedActors)
				{
					if (IsValid(overlappedActor) && !m_Owner->HasContainHitActor(m_AttackName,overlappedActor))
					{
						m_Owner->AddHitActorsByMe(m_AttackName, overlappedActor);
						m_Owner->Attack(m_AttackName, overlappedActor, weaponCollider->GetComponentLocation());
						
						if (IsValid(m_HitSound))
						{
							UGameplayStatics::PlaySound2D(m_Owner.Get(), m_HitSound, 2.5f);
						}
					}
				}
			}
		}
	}
}

void UNotifyState_Check_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (m_Owner.IsValid())
	{
		//m_Owner->ClearComponentOverlaps();
		m_Owner->GetCollider(m_ColliderName)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
