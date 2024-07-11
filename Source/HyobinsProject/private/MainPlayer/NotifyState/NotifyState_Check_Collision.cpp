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
	
	ACharacterBase* owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	if (owner != nullptr)
	{
		owner->GetCollider(m_ColliderName)->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UNotifyState_Check_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ACharacterBase* owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	if (owner != nullptr)
	{
		owner->GetCollider(m_ColliderName)->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (m_SkillName != "Empty")
		{
			owner->EmptyCheckedHitActor(m_SkillName);
		}
	}
}
