// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/HPBar.h"
#include "UI/System/EnvironmentSettings.h"
#include "UI/System/Combo.h"
#include "UI/System/Damage.h"
#include "Utility/CharacterBase.h"
#include "Utility/CustomStructs.h"


void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_MonsterHPBarClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	m_EnvironmentSettingsClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/EnvironmentSettings.EnvironmentSettings_C'"));
	m_ComboClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Combo.Combo_C'"));
	m_DamageClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Damage.Damage_C'"));
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();
}

void UUIManager::OpenEnvironmentSettings() const
{
	UEnvironmentSettings* environmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), m_EnvironmentSettingsClass));
	environmentSettings->AddToViewport();
	environmentSettings->Open();
}

void UUIManager::CreateComboWidjet()
{
	if (m_Combo != nullptr) return;
	
	m_Combo = Cast<UCombo>(CreateWidget(GetWorld(), m_ComboClass));
	m_Combo->AddToViewport();
	m_Combo->SetVisibility(ESlateVisibility::Collapsed);
}

void UUIManager::BindActorToComboWidget(ACharacterBase* const hitActor)
{
	if (m_Combo == nullptr)
	{
		CreateComboWidjet();
	}

	if (m_Combo != nullptr)
	{
		m_Combo->BindActor(hitActor);
	}
}

void UUIManager::RenderDamageToScreen(const FHitInformation& hitInfo)
{
	UDamage* damageWidget = Cast<UDamage>(CreateWidget(GetWorld(), m_DamageClass));
	
	FVector2D screenPosition;
	GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(hitInfo.hitActorLocation, screenPosition);
	screenPosition.Y -= 100.0f;
	damageWidget->SetPositionInViewport(screenPosition);
	damageWidget->SetDamage(hitInfo.damage);
	damageWidget->AddToViewport();
	damageWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	FTimerHandle destroyTimer;
	GetWorld()->GetTimerManager().SetTimer
		(
			destroyTimer,
			[=]()
			{
				damageWidget->RemoveFromParent();
			},
		1.0f,
		false);
}

void UUIManager::BindActorToDamageWidget(ACharacterBase* const hitActor)
{
	hitActor->OnTakeDamage.AddUObject(this, &UUIManager::RenderDamageToScreen);
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
	
	UUserWidget* widgetObject = widgetComponent->GetUserWidgetObject();
	UHPBar* hpBar = Cast<UHPBar>(widgetObject);
	
	hpBar->BindStatComponent(statComponent);

	if (!m_UIWidgets.Contains(m_MonsterHPBarClass))
	{
		TArray<TWeakObjectPtr<UUserWidget>> temp;
		m_UIWidgets.Add(m_MonsterHPBarClass, temp);
	}

	m_UIWidgets[m_MonsterHPBarClass].Add(widgetObject);
}

void UUIManager::ShowMonsterHPBar()
{
	for (const TWeakObjectPtr<UUserWidget> widget : m_UIWidgets[m_MonsterHPBarClass])
	{
		if (widget.IsValid())
		{
			widget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UUIManager::HideMonsterHPBar()
{
	for (const TWeakObjectPtr<UUserWidget> widget : m_UIWidgets[m_MonsterHPBarClass])
	{
		if (widget.IsValid())
		{
			widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void UUIManager::HideWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());

	for (const TWeakObjectPtr<UUserWidget> widget : m_UIWidgets[widgetClass])
	{
		if (widget.IsValid())
		{
			widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUIManager::ShowWidgets(const FName& path)
{
	UClass* const widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());

	for (const TWeakObjectPtr<UUserWidget> widget : m_UIWidgets[widgetClass])
	{
		if (widget.IsValid())
		{
			widget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UUIManager::ClearWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());

	if (widgetClass != nullptr)
	{
		m_UIWidgets[widgetClass].Empty();
	}
}

void UUIManager::ClearAllWidgets()
{
	for (auto iter : m_UIWidgets)
	{
		iter.Value.Empty();
	}
}