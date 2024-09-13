// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"

#include "Component/StatComponent.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/HPBar.h"
#include "UI/System/EnvironmentSettings.h"
#include "UI/System/Combo.h"


void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_MonsterHPBarClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	m_EnvironmentSettingsClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/EnvironmentSettings.EnvironmentSettings_C'"));
	m_ComboClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Combo.Combo_C'"));
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();
}

void UUIManager::CreateEnvironmentSettings()
{
	if (m_EnvironmentSettings != nullptr) return;
	
	m_EnvironmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), m_EnvironmentSettingsClass));
	m_EnvironmentSettings->AddToViewport();
	m_EnvironmentSettings->SetVisibility(ESlateVisibility::Collapsed);
}

UEnvironmentSettings* UUIManager::GetEnvironmentSettingsWidjet()
{
	if (m_EnvironmentSettings == nullptr)
	{
		CreateEnvironmentSettings();
	}

	return m_EnvironmentSettings;
}

void UUIManager::ShowEnvironmentSettings()
{
	if (m_EnvironmentSettings == nullptr)
	{
		CreateEnvironmentSettings();
	}
	
	m_EnvironmentSettings->SetVisibility(ESlateVisibility::Visible);
	m_EnvironmentSettings->SetKeyboardFocus();
}

void UUIManager::HideEnvironmentSettings()
{
	if (m_EnvironmentSettings == nullptr)
	{
		CreateEnvironmentSettings();
	}

	m_EnvironmentSettings->SetVisibility(ESlateVisibility::Collapsed);
}

void UUIManager::CreateComboWidjet()
{
	if (m_Combo != nullptr) return;
	
	m_Combo = Cast<UCombo>(CreateWidget(GetWorld(), m_ComboClass));
	m_Combo->AddToViewport();
	m_Combo->SetVisibility(ESlateVisibility::Collapsed);
}

UCombo* UUIManager::GetComboWidjet()
{
	if (m_Combo == nullptr)
	{
		CreateComboWidjet();
	}

	return m_Combo;
}


void UUIManager::CreateMonsterHPBar(AActor* actor, UStatComponent* const statComponent, USceneComponent* mesh, const FName& subObjectName, const FVector& relativeLocation, const FVector2D& drawSize)
{
	UWidgetComponent* const widgetComponent = NewObject<UWidgetComponent>(actor, UWidgetComponent::StaticClass(), subObjectName);
	widgetComponent->SetupAttachment(mesh);
	widgetComponent->SetRelativeLocation(relativeLocation);
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	
	widgetComponent->SetWidgetClass(m_MonsterHPBarClass);
	widgetComponent->SetDrawSize(drawSize);
	
	UUserWidget* const widgetObject = widgetComponent->GetUserWidgetObject();
	UHPBar* const hpBar = Cast<UHPBar>(widgetObject);
	
	hpBar->BindStatComponent(statComponent);

	if (!m_UIWidgets.Contains(m_MonsterHPBarClass))
	{
		TArray<UUserWidget*> temp;
		m_UIWidgets.Add(m_MonsterHPBarClass, temp);
	}

	m_UIWidgets[m_MonsterHPBarClass].Add(widgetObject);
}

void UUIManager::ShowMonsterHPBar()
{
	for (UUserWidget* const widget : m_UIWidgets[m_MonsterHPBarClass])
	{
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUIManager::HideMonsterHPBar()
{
	for (UUserWidget* const widget : m_UIWidgets[m_MonsterHPBarClass])
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UUIManager::HideWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* const widget : m_UIWidgets[widgetClass])
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUIManager::ShowWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* const widget : m_UIWidgets[widgetClass])
	{
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUIManager::ClearWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	
	m_UIWidgets[widgetClass].Empty();
}

void UUIManager::ClearAllWidgets()
{
	for (auto iter : m_UIWidgets)
	{
		iter.Value.Empty();
	}
}