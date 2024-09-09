// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/EnvironmentSettings.h"

#include "Channels/MovieSceneChannelTraits.h"
#include "SubSystems/UIManager.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"


void UEnvironmentSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_bHideCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("m_bHideCheckBox")));
	m_bHideCheckBox->OnCheckStateChanged.AddDynamic(this, &UEnvironmentSettings::ChangeMonsterHPBarState);

	m_BackGroundImage = Cast<UImage>(GetWidgetFromName(TEXT("m_BackGroundImage")));
	m_BackGroundImage->OnMouseButtonDownEvent.BindUFunction(this, FName("CloseEnvironmentSettings"));
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

FReply UEnvironmentSettings::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsFocusable = true;
	SetKeyboardFocus();
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UEnvironmentSettings::CloseEnvironmentSettings()
{
	SetKeyboardFocus();
}

FReply UEnvironmentSettings::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey inputKey = InKeyEvent.GetKey();
	FString keyString = inputKey.GetFName().ToString();
	
	if (keyString == "P")
	{
		GetOwningPlayer()->SetPause(false);
		GetOwningPlayer()->SetShowMouseCursor(false);
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}





