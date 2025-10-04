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
	
	const UDataTable* dataTable = LoadObject<UDataTable>(nullptr, *assetPath, nullptr, LOAD_None, nullptr);
	check(dataTable != nullptr);
	
	TMap<FName,FAttackInformation> attackInfos;
	
	const TArray<FName> rowNames = dataTable->GetRowNames();

	for (FName rowName : rowNames)
	{
		const FAttackInformationData data = *(dataTable->FindRow<FAttackInformationData>(rowName, rowName.ToString()));

		FAttackInformation attackInfo;
		attackInfo.attackName = data.attackName;
		attackInfo.damageRatio = data.damageRatio;
		attackInfo.crowdControlType = data.crowdControlType;
		attackInfo.crowdControlTime = data.crowdControlTime;
		attackInfo.knockBackDistance = data.knockBackDistance;
		attackInfo.airbornePower = data.airbornePower;
		attackInfo.staminaDamage = data.staminaDamage;

		attackInfos.Add(attackInfo.attackName, attackInfo);
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

const FAttackInformation* UDataManager::GetAttackInformation(TSubclassOf<AActor> classType, const FName& attackName) const
{
	if (!m_AttackInformations.Contains(classType) || !m_AttackInformations[classType].Contains(attackName))
	{
		return nullptr;
	}
		
	return &m_AttackInformations[classType][attackName];
}
