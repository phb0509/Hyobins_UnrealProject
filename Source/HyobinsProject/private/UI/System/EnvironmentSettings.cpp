// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/EnvironmentSettings.h"
#include "SubSystems/UIManager.h"
#include "Components/CheckBox.h"


void UEnvironmentSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_bHideCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("m_bHideCheckBox")));
	check(m_bHideCheckBox != nullptr);
	
	m_bHideCheckBox->OnCheckStateChanged.AddDynamic(this, &UEnvironmentSettings::ChangeMonsterHPBarState);

	if (GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->IsShowMonsterHPBar())
	{
		m_bHideCheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
	else
	{
		m_bHideCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
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
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetVisibilityWidgets("MonsterHPBar", ESlateVisibility::HitTestInvisible);
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetIsShowMonsterHPBar(true);
	}
	else
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetVisibilityWidgets("MonsterHPBar", ESlateVisibility::Collapsed);
		GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetIsShowMonsterHPBar(false);
	}
}



