// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/HPBar.h"
#include "UI/System/EnvironmentSettings.h"


void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_MonsterHPBarClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	m_EnvironmentSettingsClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Widget_SettingPopup.Widget_SettingPopup_C'"));
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();
}

void UUIManager::CreateMonsterHPBarComponent(AActor* actor, UStatComponent* const statComponent, USceneComponent* mesh, const FName& subObjectName, const FVector& relativeLocation, const FVector2D& drawSize)
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
	checkf(widgetObject != nullptr, TEXT("Failed to Get UserWidgetObject"));

	UHPBar* const hpBar = Cast<UHPBar>(widgetObject);
	checkf(hpBar != nullptr, TEXT("Failed to Cast To HPBar"));

	hpBar->BindStatComponent(statComponent);

	if (!m_UIWidgets.Contains(m_MonsterHPBarClass))
	{
		TArray<UUserWidget*> temp;
		m_UIWidgets.Add(m_MonsterHPBarClass, temp);
	}

	m_UIWidgets[m_MonsterHPBarClass].Add(widgetObject);
}

void UUIManager::CreateEnvironmentSettings()
{
	if (m_EnvironmentSettings != nullptr) return;
	
	m_EnvironmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), m_EnvironmentSettingsClass));
	m_EnvironmentSettings->AddToViewport();
	m_EnvironmentSettings->SetVisibility(ESlateVisibility::Collapsed);
}

void UUIManager::HideWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* const widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("Widget is Nullptr!!"));
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUIManager::ShowWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* const widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("Widget is Nullptr!!"));
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUIManager::ClearWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	m_UIWidgets[widgetClass].Empty();
}

void UUIManager::ClearAllWidgets()
{
	for (auto iter : m_UIWidgets)
	{
		iter.Value.Empty();
	}
}

UEnvironmentSettings* UUIManager::GetEnvironmentSettings()
{
	if (m_EnvironmentSettings == nullptr)
	{
		CreateEnvironmentSettings();
	}

	return m_EnvironmentSettings;
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
