// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Move_ReceiveInput.h"
#include "MainPlayer/MainPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UNotifyState_Move_ReceiveInput::UNotifyState_Move_ReceiveInput() :
m_MoveOffset(5000.0f),
m_CurInputHorizontal(0),
m_CurInputVertical(0)
{
}

void UNotifyState_Move_ReceiveInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	m_Owner = Cast<AMainPlayer>(MeshComp->GetOwner());
	if (m_Owner != nullptr)
	{
		m_CurInputHorizontal = m_Owner->GetCurInputHorizontal();
		m_CurInputVertical = m_Owner->GetCurInputVertical();
	}
}

void UNotifyState_Move_ReceiveInput::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (m_Owner != nullptr)
	{
		m_Owner->GetCharacterMovement()->AddImpulse(m_Owner->GetActorForwardVector() * m_MoveOffset * m_CurInputVertical * FrameDeltaTime, true);
		m_Owner->GetCharacterMovement()->AddImpulse(m_Owner->GetActorRightVector() * m_MoveOffset * m_CurInputHorizontal * FrameDeltaTime, true);
	}
}
