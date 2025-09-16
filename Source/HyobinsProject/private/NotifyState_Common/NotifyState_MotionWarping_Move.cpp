// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Common/NotifyState_MotionWarping_Move.h"
#include "CharacterBase/CharacterBase.h"
#include "MotionWarpingComponent.h"

UNotifyState_MotionWarping_Move::UNotifyState_MotionWarping_Move(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_Forward(0),
	m_Right(0),
	m_Up(0),
	m_MoveDistance(0.0f)
{
}

void UNotifyState_MotionWarping_Move::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacterBase* owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (owner != nullptr)
	{
		const FVector forwardVector = owner->GetActorForwardVector() * m_MoveDistance * m_Forward;
		const FVector rightVector = owner->GetActorRightVector() * m_MoveDistance * m_Right;
		const FVector upVector = owner->GetActorUpVector() * m_MoveDistance * m_Up;

		UMotionWarpingComponent* component = owner->GetMotionWarpingComponent();
		
		if (component != nullptr)
		{
			component->AddOrUpdateWarpTargetFromLocation(
				TEXT("Forward"), owner->GetActorLocation() + rightVector + forwardVector + upVector);
		}
	}
}

void UNotifyState_MotionWarping_Move::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	
}




