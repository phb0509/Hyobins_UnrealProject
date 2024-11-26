// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



class HYOBINSPROJECT_API Utility
{
public:
	Utility();
	~Utility();
	
	template<class T>
	static T* NewBlueprintObjectInConstructor(const FString assetPath, UObject* outer)
	{
		static ConstructorHelpers::FObjectFinder<UBlueprint> object(*assetPath);
		UClass* blueprintClass = object.Object->GeneratedClass;
		
		if (blueprintClass->IsChildOf(T::StaticClass()))
		{
			return Cast<T>(NewObject<UObject>(outer, blueprintClass));
		}

		return nullptr;
	}
	
	// template<class T>
	// static T* FindClass(const FString assetPath)
	// {
	// 	static ConstructorHelpers::FObjectFinder<T*> object(*assetPath);
	// 	
	// 	return object;
	// }
	
	FORCEINLINE static double ConvertToDegree(int inputVertical, int inputHorizontal) { return m_DegreeTable[inputVertical + 1][inputHorizontal + 1]; }
	static int32 GetHitDirection(const AActor* hitActor, const AActor* attackActor); // 상하좌우 4방향 판별.
	
	template <typename T> 
	static FString ConvertEnumToString(const T enumType) 
	{
		return StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(enumType)); 
	}

	template <typename T> 
	static FName ConvertEnumToName(const T enumType) 
	{ 
		return FName(*StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(enumType))); 
	}

private:
	static const double m_DegreeTable[3][3];

};


