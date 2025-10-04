// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NotifyState/Move.h"

UMove::UMove() :
	m_MoveDistance(0.0f),
	m_Duration(0.0f),
	m_ElapsedTime(0.0f),
	m_DistancePerSecond(0.0f) 
{
}

void UMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* owner = MeshComp->GetOwner();
	check(owner != nullptr);
	
	m_StartLocation = owner->GetActorLocation(); // 시작위치
	m_TargetLocation = m_StartLocation + (owner->GetActorForwardVector() * m_MoveDistance);
	m_Duration = TotalDuration; // 노티파이스테이트 범위 총 시간
}

void UMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AActor* owner = MeshComp->GetOwner();
	check(owner != nullptr);
	
	m_ElapsedTime += FrameDeltaTime; // 경과 시간 누적
	float alpha = FMath::Clamp(m_ElapsedTime / m_Duration, 0.0f, 1.0f);
	
	FVector newLocation = FMath::Lerp(m_StartLocation, m_TargetLocation, alpha);
	
	owner->SetActorLocation(newLocation);
}


