// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



class HYOBINSPROJECT_API Utility
{
public:
	Utility();
	~Utility();
	
	FORCEINLINE static double ConvertToDegree(int inputVertical, int inputHorizontal) { return m_DegreeTable[inputVertical + 1][inputHorizontal + 1]; }
	FORCEINLINE static int32 GetDirectionIndex(const int inputVertical, const int inputHorizontal) { return m_DirectionIndex[inputVertical + 1][inputHorizontal + 1]; }
	static int32 GetHitDirection(const AActor* hitActor, const AActor* attackActor);
	
	template <typename T> 
	static FORCEINLINE FString ConvertEnumToString(const T enumType) 
	{ 
		return StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(enumType)); 
	}

	template <typename T> 
	static FORCEINLINE FName ConvertEnumToName(const T enumType) 
	{ 
		return FName(*StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(enumType))); 
	}
	
	
private:
	static const double m_DegreeTable[3][3];
	static const int32 m_DirectionIndex[3][3];

};
