// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Notify/RotateToTarget.h"
#include "Monster/Monster.h"


URotateToTarget::URotateToTarget() :
	m_RotatorOffset({0.0f, 0.0f, 0.0f})
{
}

void URotateToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AMonster* owner = Cast<AMonster>(MeshComp->GetOwner());

	if (owner != nullptr)
	{
		ACharacterBase* target = owner->GetTarget();

		if (target != nullptr)
		{
			owner->RotateToTarget(target, m_RotatorOffset);
		}
	}
}
