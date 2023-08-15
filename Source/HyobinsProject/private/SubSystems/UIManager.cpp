// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "GameFramework/Actor.h"
#include "Utility/StatusActor.h"
#include "Components/WidgetComponent.h"
#include "UI/CharacterUpperHPBar.h"
#include "Components/SceneComponent.h"

void UUIManager::CreateUpperHPBar(AActor* actor, USceneComponent* mesh, FName subObjectName, FString assetPath, FVector relativeLocation, FVector2D drawSize)
{
	checkf(actor->GetClass()->ImplementsInterface(UStatusActor::StaticClass()), TEXT("Actor doesn't inherit StatusActor interfaces."));
	IStatusActor* castedStatusActor = Cast<IStatusActor>(actor);

	//UWidgetComponent* widgetComponent = CreateDefaultSubobject<UWidgetComponent>(*assetPath);
	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(actor, UWidgetComponent::StaticClass(), subObjectName);
	widgetComponent->SetupAttachment(mesh);
	widgetComponent->SetRelativeLocation(relativeLocation);
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());

	/*static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(*assetPath);
	if (UI_HUD.Succeeded())
	{
		widgetComponent->SetWidgetClass(UI_HUD.Class);
		widgetComponent->SetDrawSize(drawSize);
	}*/
	//WidgetBlueprint'/Game/UI/Monster/UI_HPBar.UI_HPBar'
	
	//UUserWidget* userWidget = LoadObject<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/UI_HPBar.UI_HPBar'"));
	//auto userWidget = LoadObject<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UI/Monster/UI_HPBar.UI_HPBar'"), NULL, LOAD_None, NULL);
	auto tClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/UI_HPBar.UI_HPBar_C'"));

	if (tClass != nullptr)
	{
		widgetComponent->SetWidgetClass(tClass);
		widgetComponent->SetDrawSize(drawSize);
	}

	auto widgetObject = widgetComponent->GetUserWidgetObject();
	UCharacterUpperHPBar* upperHPBar = Cast<UCharacterUpperHPBar>(widgetObject);
	if (upperHPBar != nullptr)
	{
		
		upperHPBar->BindStatComponent(castedStatusActor->GetStatComponent());
	}



	// TMap<TWeakObjectPtr<class AActor>, TMap<TSubclassOf<class UUserWidget>, TWeakObjectPtr<class UUserWidget>>> m_UIWidgets;
}