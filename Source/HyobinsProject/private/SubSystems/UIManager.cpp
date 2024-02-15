// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "GameFramework/Actor.h"
#include "Utility/StatActor.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/HPBar.h"

void UUIManager::CreateHPBarComponent(AActor* actor, USceneComponent* mesh, const FName& subObjectName, const FString& assetPath, const FVector& relativeLocation, const FVector2D& drawSize)
{
	checkf(actor->GetClass()->ImplementsInterface(UStatActor::StaticClass()), TEXT("Actors don't inherit StatusActor interfaces."));
	IStatActor* castedStatActor = Cast<IStatActor>(actor);

	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(actor, UWidgetComponent::StaticClass(), subObjectName);
	widgetComponent->SetupAttachment(mesh);
	widgetComponent->SetRelativeLocation(relativeLocation);
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());

	TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	if (widgetClass != nullptr)
	{
		widgetComponent->SetWidgetClass(widgetClass);
		widgetComponent->SetDrawSize(drawSize);
	}

	UUserWidget* widgetObject = widgetComponent->GetUserWidgetObject();
	checkf(widgetObject != nullptr, TEXT("Failed to Get UserWidgetObject"));

	UHPBar* hpBar = Cast<UHPBar>(widgetObject);
	checkf(hpBar != nullptr, TEXT("Failed to Cast To HPBar"));

	hpBar->BindStatComponent(castedStatActor->GetStatComponent());

	if (m_UIWidgets.Contains(widgetClass) == false)
	{
		TArray<class UUserWidget*> temp;
		m_UIWidgets.Add(widgetClass, temp);
	}

	m_UIWidgets[widgetClass].Add(widgetObject);
}

void UUIManager::HideWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("Widget is Nullptr!!"));
		widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUIManager::ShowWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("Widget is Nullptr!!"));
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUIManager::ClearWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
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
