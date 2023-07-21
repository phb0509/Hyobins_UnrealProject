// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class HYOBINSPROJECT_API Utility
{
public:
	Utility();
	~Utility();

	static double ConvertToDegree(int inputVertical, int inputHorizontal) { return m_DegreeTable[inputVertical + 1][inputHorizontal + 1]; }
	static int32 GetHitDirection(AActor* hitActor, AActor* attackActor);

private:
	static const double m_DegreeTable[3][3];

};
