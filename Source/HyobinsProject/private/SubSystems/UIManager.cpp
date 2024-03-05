// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/HPBar.h"

void UUIManager::CreateHPBarComponent(AActor* actor, UStatComponent* const statComponent, USceneComponent* mesh, const FName& subObjectName, const FString& assetPath, const FVector& relativeLocation, const FVector2D& drawSize)
{
	UWidgetComponent* const widgetComponent = NewObject<UWidgetComponent>(actor, UWidgetComponent::StaticClass(), subObjectName);
	widgetComponent->SetupAttachment(mesh);
	widgetComponent->SetRelativeLocation(relativeLocation);
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());

	const TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));
	
	widgetComponent->SetWidgetClass(widgetClass);
	widgetComponent->SetDrawSize(drawSize);
	
	UUserWidget* const widgetObject = widgetComponent->GetUserWidgetObject();
	checkf(widgetObject != nullptr, TEXT("Failed to Get UserWidgetObject"));

	UHPBar* const hpBar = Cast<UHPBar>(widgetObject);
	checkf(hpBar != nullptr, TEXT("Failed to Cast To HPBar"));

	hpBar->BindStatComponent(statComponent);

	if (m_UIWidgets.Contains(widgetClass) == false)
	{
		TArray<UUserWidget*> temp;
		m_UIWidgets.Add(widgetClass, temp);
	}

	m_UIWidgets[widgetClass].Add(widgetObject);
}

void UUIManager::HideWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* const widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("Widget is Nullptr!!"));
		widget->SetVisibility(ESlateVisibility::Hidden);
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
