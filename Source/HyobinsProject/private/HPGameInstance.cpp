// Fill out your copyright notice in the Description page of Project Settings.


#include "HPGameInstance.h"
#include "Utility/EnumTypes.h"


UHPGameInstance::UHPGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PlayerAttackInformations(*CharacterDataPath);
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

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_AttackInformations(*assetPath);

	if (DT_AttackInformations.Succeeded())
	{
		UDataTable* temp = DT_AttackInformations.Object;

		temp->
	}
	

}
