// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Utility.h"


const double Utility::m_DegreeTable[3][3] = { {-135,180,135},{-90,0,90},{-45,0,45} };

Utility::Utility() 
{

}

Utility::~Utility()
{

}

int32 Utility::GetHitDirection(const AActor* hitActor, const AActor* attackActor) // 공격당한 액터, 공격한 액터
{
	FVector forward = hitActor->GetActorForwardVector();
	FVector toActor = attackActor->GetActorLocation() - hitActor->GetActorLocation(); // hitActor -> attackActor 벡터.
	toActor.Normalize();

	float cosTheta = FVector::DotProduct(forward, toActor);
	float theta = FMath::Acos(cosTheta);

	FVector cross = FVector::CrossProduct(forward, toActor);

	if (theta >= 0.0f && theta < PI / 4) // Front
	{
		return 0;
	}
	else if (cross.Z >= 0.0f && theta >= PI / 4 && theta < PI * 3 / 4) // Right
	{
		return 1;
	}
	else if (theta >= PI * 3 / 4 && theta < PI) // Back
	{
		return 2;
	}
	else if (cross.Z < 0 && theta >= PI / 4 && theta < PI * 3 / 4) // Left
	{
		return 3;
	}
	
	return -1;
}



