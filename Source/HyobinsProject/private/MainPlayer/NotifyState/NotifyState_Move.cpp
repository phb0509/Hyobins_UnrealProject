// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Move.h"

UNotifyState_Move::UNotifyState_Move() :
m_MoveOffset(3000.0f),
m_HorizontalValue(0),
m_VerticalValue(0)
{
}

void UNotifyState_Move::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}


