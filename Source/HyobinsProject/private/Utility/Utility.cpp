// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Utility.h"


const double Utility::m_DegreeTable[3][3] = { {-135,180,135},{-90,0,90},{-45,0,45} };
const int32 Utility::m_DirectionIndex[3][3] = {{5,4,3}, {6,0,2}, {7,0,1}};

Utility::Utility()
{

}

Utility::~Utility()
{
}

int32 Utility::GetHitDirection(const AActor* hitActor, const AActor* attackActor) // 공격당한 액터, 공격한 액터
{
	const FVector forward = hitActor->GetActorForwardVector();
	FVector toActor = attackActor->GetActorLocation() - hitActor->GetActorLocation(); // hitActor -> attackActor 벡터.
	toActor.Normalize();

	const float cosTheta = FVector::DotProduct(forward, toActor);
	const float theta = FMath::Acos(cosTheta);
	const FVector cross = FVector::CrossProduct(forward, toActor);

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

void Utility::InitAttackInformations(const FString& assetPath, TMap<FName, FAttackInfoStruct>& attackInformations)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Object(*assetPath);

	if (DT_Object.Succeeded())
	{
		const UDataTable* const DT_AttackInformations = DT_Object.Object;
		const TArray<FName> attackNames = DT_AttackInformations->GetRowNames();

		for (auto& attackName : attackNames)
		{
			const FHPAttackInformationData data =
				*(DT_AttackInformations->FindRow<FHPAttackInformationData>(attackName, attackName.ToString()));

			ECrowdControlType crowdControlType = ECrowdControlType::None;

			const UEnum* crowdControlEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);

			if (crowdControlEnum != nullptr)
			{
				const int32 index = crowdControlEnum->GetIndexByName(data.crowdControlType);
				crowdControlType = ECrowdControlType(index);
			}

			FAttackInfoStruct attackInfoStruct;
			attackInfoStruct.attackName = attackName;
			attackInfoStruct.damage = data.damage;
			attackInfoStruct.bIsDot = data.bIsDot;
			attackInfoStruct.bHasCrowdControl = data.bHasCrowdControl;
			attackInfoStruct.crowdControlType = crowdControlType;
			attackInfoStruct.crowdControlTime = data.crowdControlTime;
			attackInfoStruct.bHasKnockBack = data.bHasKnockBack;
			attackInfoStruct.knockBackTime = data.knockBackTime;
			attackInfoStruct.knockBackDistance = data.knockBackDistance;

			attackInformations.Add(attackName, attackInfoStruct);
		}
	}
}



