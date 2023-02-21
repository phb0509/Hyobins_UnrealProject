// Fill out your copyright notice in the Description page of Project Settings.


#include "HPGameInstance.h"
#include "Utility/EnumTypes.h"

UHPGameInstance::UHPGameInstance()
{
}

void UHPGameInstance::Init()
{
	Super::Init();
}

void UHPGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UHPGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

void UHPGameInstance::InitAttackInformations(IN FString assetPath, OUT TMap<FName, FAttackInfoStruct>& attackInformations)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Object(*assetPath);

	if (DT_Object.Succeeded())
	{
		UDataTable* DT_AttackInformations = DT_Object.Object;

		TArray<FName> attackNames = DT_AttackInformations->GetRowNames();

		for (auto& attackName : attackNames)
		{
			FHPAttackInformationData data =
				*(DT_AttackInformations->FindRow<FHPAttackInformationData>(attackName, attackName.ToString()));

			ECrowdControlType crowdControlType;

			const UEnum* crowdControlEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);

			if (crowdControlEnum)
			{
				int32 Index = crowdControlEnum->GetIndexByName(data.crowdControlType);
				crowdControlType = ECrowdControlType((uint8)Index);
			}

			attackInformations.Add(attackName, { data.damage,data.bIsDot,data.bHasCrowdControl,crowdControlType,data.crowdControlTime,data.bHasKnockBack,data.knockBackTime,data.knockBackDistance });
		}
	}
}
