// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Utility.h"


const double Utility::m_DegreeTable[3][3] =
	{ 
	{-135,180,135},
	{-90,0,90},
	{-45,0,45} 
	};

Utility::Utility()
{
}

Utility::~Utility()
{
}

int32 Utility::GetHitDirection(const FVector& instigatorLocation, const AActor* hitActor) // 공격한 액터위치, 공격당한 액터
{
	if (IsValid(hitActor))
	{
		return 0;
	}
	
	const FVector forward = hitActor->GetActorForwardVector();
	FVector toActor = instigatorLocation - hitActor->GetActorLocation(); // hitActor -> attackActor 벡터.
	toActor.Normalize();
	
	const float cosTheta = FVector::DotProduct(forward, toActor);
	const float theta = FMath::Acos(cosTheta); // 전방벡터기준 플레이어
	const FVector cross = FVector::CrossProduct(forward, toActor);
	
	if (theta >= 0.0f && theta < PI / 4) // Front
	{
		return 0;
	}
	
	if (cross.Z >= 0.0f && theta >= PI / 4 && theta < PI * 3 / 4) // Right
	{
		return 1;
	}
	
	if (theta >= PI * 3 / 4 && theta < PI) // Back
	{
		return 2;
	}
	
	if (cross.Z < 0 && theta >= PI / 4 && theta < PI * 3 / 4) // Left
	{
		return 3;
	}
	
	return -1;
}


