// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class HYOBINSPROJECT_API Utility
{
public:
	Utility();
	~Utility();
	
	
	FORCEINLINE static double ConvertToDegree(int inputVertical, int inputHorizontal)
	{
		return m_DegreeTable[inputVertical + 1][inputHorizontal + 1];
	}
	
	static int32 GetHitDirection(const FVector& instigatorLocation, const AActor* hitActor); // 상하좌우 4방향 판별.
	
	template <typename T> 
	static FString ConvertEnumToString(const T enumType) 
	{
		return StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(enumType)); 
	}


private:
	static const double m_DegreeTable[3][3];

};


