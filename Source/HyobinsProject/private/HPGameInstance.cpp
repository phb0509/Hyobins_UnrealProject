// Fill out your copyright notice in the Description page of Project Settings.


#include "HPGameInstance.h"
#include "Utility/EnumTypes.h"
#include "ActorPool/ActorPool.h"


UHPGameInstance::UHPGameInstance()
{
	m_Enums.Add("ENormalMinionStates", FindObject<UEnum>(ANY_PACKAGE, TEXT("ENormalMinionStates"), true));
}

void UHPGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Call the HPGameInstance::Init!"));

	m_ActorPool = GetWorld()->SpawnActor<AActorPool>();
	checkf(IsValid(m_ActorPool), TEXT("UHPGameInstance::m_ActorPool isn't Valid"));
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

void UHPGameInstance::InitAttackInformations(IN const FString& assetPath, OUT TMap<FName, FAttackInfoStruct>& attackInformations)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Object(*assetPath);

	if (DT_Object.Succeeded())
	{
		const UDataTable* DT_AttackInformations = DT_Object.Object;

		TArray<FName> attackNames = DT_AttackInformations->GetRowNames();

		for (auto& attackName : attackNames)
		{
			const FHPAttackInformationData data =
				*(DT_AttackInformations->FindRow<FHPAttackInformationData>(attackName, attackName.ToString()));

			ECrowdControlType crowdControlType = ECrowdControlType::None;

			const UEnum* crowdControlEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);

			if (crowdControlEnum != nullptr)
			{
				const int32 index = crowdControlEnum->GetIndexByName(data.crowdControlType);
				crowdControlType = ECrowdControlType((uint8)index);
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

int32 UHPGameInstance::GetIndexByEnumName(const FName& enumClassName, const FName& enumTypeName) const
{
	return m_Enums[enumClassName]->GetIndexByName(enumTypeName);
}

