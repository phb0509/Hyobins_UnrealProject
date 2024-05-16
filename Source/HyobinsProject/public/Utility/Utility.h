// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/CustomStructs.h"


class HYOBINSPROJECT_API Utility
{
public:
	Utility();
	~Utility();
	
	FORCEINLINE static double ConvertToDegree(int inputVertical, int inputHorizontal) { return m_DegreeTable[inputVertical + 1][inputHorizontal + 1]; }
	FORCEINLINE static int32 GetDirectionIndex(const int inputVertical, const int inputHorizontal) { return m_DirectionIndex[inputVertical + 1][inputHorizontal + 1]; }
	static int32 GetHitDirection(const AActor* hitActor, const AActor* attackActor);
	static void InitAttackInformations(IN const FString& assetPath, OUT TMap<FName, FAttackInfo>& attackInformations);

	
private:
	static const double m_DegreeTable[3][3];
	static const int32 m_DirectionIndex[3][3];

};
