// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "GameFramework/Actor.h"
#include "Utility/StatusActor.h"
#include "Components/WidgetComponent.h"
#include "UI/HPBar.h"
#include "Components/SceneComponent.h"

void UUIManager::CreateHPBarComponent(AActor* actor, USceneComponent* mesh, FName subObjectName, FString assetPath, FVector relativeLocation, FVector2D drawSize)
{
	checkf(actor->GetClass()->ImplementsInterface(UStatusActor::StaticClass()), TEXT("Actors don't inherit StatusActor interfaces."));
	IStatusActor* castedStatusActor = Cast<IStatusActor>(actor);

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

	hpBar->BindStatComponent(castedStatusActor->GetStatComponent());

	if (m_UIWidgets.Contains(widgetClass) == false)
	{
		TArray<class UUserWidget*> temp;
		m_UIWidgets.Add(widgetClass, temp);
	}

	m_UIWidgets[widgetClass].Add(widgetObject);
}

void UUIManager::HideWidgets(FName path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("widget is Nullptr"));
		widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUIManager::ShowWidgets(FName path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	checkf(widgetClass != nullptr, TEXT("Failed to Load WidgetClass"));

	for (UUserWidget* widget : m_UIWidgets[widgetClass])
	{
		checkf(widget != nullptr, TEXT("widget is Nullptr"));
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
