// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/DataManager.h"
#include "Utility/EnumTypes.h"


void UDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDataManager::LoadAttackInformation(TSubclassOf<AActor> classType, const FString& assetPath)
{
	if (m_AttackInformations.Contains(classType))
	{
		return;
	}
	
	const UDataTable* dataTable = LoadObject<UDataTable>(NULL, *assetPath, NULL, LOAD_None, NULL);
	TMap<FName,FAttackInformation> attackInfos;
	
	if (dataTable != nullptr)
	{
		const TArray<FName> rowNames = dataTable->GetRowNames();

		for (FName rowName : rowNames)
		{
			const FAttackInformationData data = *(dataTable->FindRow<FAttackInformationData>(rowName, rowName.ToString()));
			ECrowdControlType crowdControlType = ECrowdControlType::None;
			const UEnum* crowdControlEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);
			
			if (crowdControlEnum != nullptr)
			{
				const int32 index = crowdControlEnum->GetIndexByName(data.crowdControlType);
				crowdControlType = static_cast<ECrowdControlType>(index);
			}

			FAttackInformation attackInfo;
			attackInfo.attackName = data.attackName;
			attackInfo.damageRatio = data.damageRatio;
			attackInfo.crowdControlType = crowdControlType;
			attackInfo.crowdControlTime = data.crowdControlTime;
			attackInfo.knockBackDistance = data.knockBackDistance;
			attackInfo.airbornePower = data.airbornePower;

			attackInfos.Add(attackInfo.attackName, attackInfo);
		}
	}

	m_AttackInformations.Add(classType, attackInfos);
}

void UDataManager::DeleteAttackInformation(TSubclassOf<AActor> classType)
{
	m_AttackInformations.Remove(classType);
}

void UDataManager::InitHitActors(TSubclassOf<AActor> classType, OUT TMap<FName, TSet<TWeakObjectPtr<AActor>>>& hitActors)
{
	for (auto& iter : m_AttackInformations[classType])
	{
		const FName& attackName = iter.Key;
		TSet<TWeakObjectPtr<AActor>> temp;

		hitActors.Add(attackName, temp);
	}
}
