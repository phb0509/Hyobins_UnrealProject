// Fill out your copyright notice in the Description page of Project Settings.


#include "HPGameInstance.h"
#include "Utility/EnumTypes.h"

UHPGameInstance::UHPGameInstance()
{
	m_Enums.Add("ENormalMinionStates", FindObject<UEnum>(ANY_PACKAGE, TEXT("ENormalMinionStates"), true));
}

void UHPGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Call the HPGameInstance::Init!"));
}

void UHPGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(LogTemp, Warning, TEXT("Call the HPGameInstance::Shutdown!"));
}

void UHPGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
	UE_LOG(LogTemp, Warning, TEXT("Call the HPGameInstance::StartGameInstance!"));
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

			FAttackInfoStruct temp;
			temp.damage = data.damage;
			temp.bIsDot = data.bIsDot;
			temp.bHasCrowdControl = data.bHasCrowdControl;
			temp.crowdControlType = crowdControlType;
			temp.crowdControlTime = data.crowdControlTime;
			temp.bHasKnockBack = data.bHasKnockBack;
			temp.knockBackTime = data.knockBackTime;
			temp.knockBackDistance = data.knockBackDistance;

			attackInformations.Add(attackName,temp);
		}
	}
}

int32 UHPGameInstance::GetIndexByEnumName(FName& enumClassName, FName& enumTypeName)
{
	return m_Enums[enumClassName]->GetIndexByName(enumTypeName);
}
