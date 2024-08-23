// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Check_Collision.h"
#include "Utility/CharacterBase.h"
#include "Components/ShapeComponent.h"

UNotifyState_Check_Collision::UNotifyState_Check_Collision()
{
}

void UNotifyState_Check_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	m_Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	if (m_Owner != nullptr)
	{
		m_Owner->GetCollider(m_ColliderName)->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		if (m_AttackName != "Empty")
		{
			m_Owner->EmptyHitActorsByMe(m_AttackName);
		}
	}
}

void UNotifyState_Check_Collision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (m_Owner != nullptr)
	{
		TArray<AActor*> overlappingEnemies;
		m_Owner->GetCollider(m_ColliderName)->GetOverlappingActors(overlappingEnemies);

		if (overlappingEnemies.Num() > 0)
		{
			for (AActor* overlappingEnemy : overlappingEnemies)
			{
				if (!m_Owner->HasContainHitActor(m_AttackName,overlappingEnemy))
				{
					m_Owner->AddHitActorsByMe(m_AttackName, overlappingEnemy);
					m_Owner->Attack(m_AttackName, overlappingEnemy);
				}
			}
		}
	}
}

void UNotifyState_Check_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (m_Owner != nullptr)
	{
		m_Owner->GetCollider(m_ColliderName)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
