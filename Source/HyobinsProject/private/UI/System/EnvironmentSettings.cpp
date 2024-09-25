// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/EnvironmentSettings.h"
#include "SubSystems/UIManager.h"
#include "Components/CheckBox.h"


void UEnvironmentSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_bHideCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("m_bHideCheckBox")));
	m_bHideCheckBox->OnCheckStateChanged.AddDynamic(this, &UEnvironmentSettings::ChangeMonsterHPBarState);
}

FReply UEnvironmentSettings::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::P)
	{
		Close();
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UEnvironmentSettings::Open()
{
	GetOwningPlayer()->SetPause(true);
	GetOwningPlayer()->SetShowMouseCursor(true);
	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
	SetKeyboardFocus();
}

void UEnvironmentSettings::Close()
{
	GetOwningPlayer()->SetPause(false);
	GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	
	this->RemoveFromParent();
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



