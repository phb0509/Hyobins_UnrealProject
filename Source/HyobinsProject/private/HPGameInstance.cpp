// Fill out your copyright notice in the Description page of Project Settings.


#include "HPGameInstance.h"
#include "Utility/EnumTypes.h"


UHPGameInstance::UHPGameInstance()
{
	/*FString CharacterDataPath = TEXT("DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PlayerAttackInformations(*CharacterDataPath);*/
	//m_HBAttackInformationDataTable = DT_PlayerAttackInformations.Object;
}

void UHPGameInstance::Init()
{
	Super::Init();
}

void UHPGameInstance::InitAttackInformations(IN FString assetPath, OUT TMap<FString, FAttackInfoStruct>& attackInformationStruct)
{
	/*None    UMETA(DisplayName = "None"),
	Slow	UMETA(DisplayName = "Slow"),
	Bind	UMETA(DisplayName = "Bind"),
	Stun	UMETA(DisplayName = "Stun"),
	Airborne UMETA(DisplayName = "Airborne")*/

	//m_AttackInformations.Add("NormalAttack", { 20.0f,false,false,ECrowdControlType::None,0.0f,false,0.5f,10.0f });

	//float damage;
	//bool bIsDot;
	//bool bHasCrowdControl;
	//ECrowdControlType crowdControlType; // CC기 종류
	//float crowdControlTime; // CC기 지속시간

	//bool bHasKnockBack;
	//float knockBackTime; // 경직시간 -> 피격체의 힛트모션 재생시간과 대응
	//float knockBackDistance; // 밀려지는 정도

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Object(*assetPath);

	if (DT_Object.Succeeded())
	{
		UDataTable* DT_AttackInformations = DT_Object.Object;

		TArray<FName> rowNames = DT_AttackInformations->GetRowNames();

		for (auto& rowName : rowNames)
		{
			FHPAttackInformationData data =
				*(DT_AttackInformations->FindRow<FHPAttackInformationData>(rowName, rowName.ToString()));

			//ECrowdControlType tempType = data.crowdControlType;

			

			int a = 0;
			//float damage;
			//bool bIsDot;
			//bool bHasCrowdControl;
			//ECrowdControlType crowdControlType; // CC기 종류
			//float crowdControlTime; // CC기 지속시간

			//bool bHasKnockBack;
			//float knockBackTime; // 경직시간 -> 피격체의 힛트모션 재생시간과 대응
			//float knockBackDistance; // 밀려지는 정도

			
		
		}
	}
}

constexpr int32 UHPGameInstance::ConvertStringToConstInt(FString string)
{
	return **string ? static_cast<int32>(**string) + 33 * ConvertStringToConstInt(*string + 1) : 5381;
}
