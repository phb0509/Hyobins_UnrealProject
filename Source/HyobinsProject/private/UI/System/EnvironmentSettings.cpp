// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/EnvironmentSettings.h"
#include "SubSystems/UIManager.h"
#include "Components/CheckBox.h"


void UEnvironmentSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_bHideCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("m_bHideCheckBox")));
	UE_LOG(LogTemp, Warning, TEXT("UEnvironmentSettings :: NOT NativeConstruct"));

	
	m_bHideCheckBox->OnCheckStateChanged.AddDynamic(this, &UEnvironmentSettings::ChangeMonsterHPBarState);
}


void UEnvironmentSettings::ChangeMonsterHPBarState(bool bIsChecked)
{
	
	if (bIsChecked)
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ShowMonsterHPBar();
	}
	else
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->HideMonsterHPBar();
	}
}
