// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "Utility/CharacterBase.h"
#include "Utility/CustomStructs.h"

#include "Components/WidgetComponent.h"
#include "UI/Monster/HPBar.h"
#include "UI/System/EnvironmentSettings.h"
#include "UI/System/Combo.h"
#include "UI/System/Damage.h"
#include "UI/MainPlayer/ChargingGageBar.h"
#include "UI/MainPlayer/SkillSlots.h"
#include "UI/MainPlayer/MainPlayerStatusBar.h"
#include "UI/Monster/BossStatusBar.h"


void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	m_bIsShowMonsterHPBar = true;
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();

	RemoveAllWidgetContainers();
}

void UUIManager::CreateMainPlayerStatusBar(UStatComponent* statComponent, ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/StatusBar.StatusBar_C'"));
	UMainPlayerStatusBar* mainPlayerStatusBar = Cast<UMainPlayerStatusBar>(CreateWidget(GetWorld(), classType));
	mainPlayerStatusBar->BindStatComponent(statComponent);
	mainPlayerStatusBar->AddToViewport();
	
	addWidgetContainer(TEXT("MainPlayerStatusBar"), widgetOwner, mainPlayerStatusBar, nullptr);

}

void UUIManager::CreateBossStatusBar(UStatComponent* statComponent, ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/BossStatusBar.BossStatusBar_C'"));
	UBossStatusBar* bossStatusBar = Cast<UBossStatusBar>(CreateWidget(GetWorld(), classType));
	bossStatusBar->BindStatComponent(statComponent);
	bossStatusBar->AddToViewport();
	
	addWidgetContainer(TEXT("BossStatusBar"), widgetOwner, bossStatusBar, nullptr);
}

void UUIManager::CreateSkillSlots(USkillComponent* skillComponent, ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/SkillSlots.SkillSlots_C'"));
	USkillSlots* skillSlots = Cast<USkillSlots>(CreateWidget(GetWorld(), classType));
	skillSlots->AddToViewport();
	addWidgetContainer(TEXT("SkillSlots"), widgetOwner, skillSlots, nullptr);
	
	skillSlots->CreateSkillListFromSkillComponent(skillComponent);
}

void UUIManager::ChangeSkillList()
{
	USkillSlots* skillSlots = Cast<USkillSlots>(m_UIWidgetsWidgetNameKey["SkillSlots"][0].widget);
	skillSlots->ChangeSkillSlots();
}

void UUIManager::OpenEnvironmentSettings()
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/EnvironmentSettings.EnvironmentSettings_C'"));
	UEnvironmentSettings* environmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), classType));
	environmentSettings->AddToViewport();
	addWidgetContainer(TEXT("EnvironmentSettings"), this, environmentSettings, nullptr);
	
	environmentSettings->Open();
}

UCombo* UUIManager::CreateComboWidget()
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Combo.Combo_C'"));
	UCombo* combo = Cast<UCombo>(CreateWidget(GetWorld(), classType));
	combo->AddToViewport();
	addWidgetContainer(TEXT("Combo"), this, combo, nullptr);
	
	combo->SetVisibility(ESlateVisibility::Collapsed);

	return combo;
}

void UUIManager::BindActorToComboWidget(ACharacterBase* hitActor)
{
	UCombo* combo = CreateComboWidget();
	
	hitActor->OnTakeDamage.AddUObject(combo, &UCombo::UpdateComboCount);
}

void UUIManager::BindActorToDamageWidget(ACharacterBase* hitActor)
{
	hitActor->OnTakeDamage.AddUObject(this, &UUIManager::RenderDamageToScreen);
}

void UUIManager::RenderDamageToScreen(const FHitInformation& hitInfo)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Damage.Damage_C'"));
	UDamage* damageWidget = Cast<UDamage>(CreateWidget(GetWorld(), classType));
	damageWidget->AddToViewport();
	
	FVector2D screenPosition;
	GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(hitInfo.hitActorLocation, screenPosition);
	screenPosition.Y -= 100.0f;
	damageWidget->SetPositionInViewport(screenPosition);
	damageWidget->SetDamage(hitInfo.damage);
	damageWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUIManager::CreateMonsterHPBar(ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	
	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(
		widgetOwner, UWidgetComponent::StaticClass(), "UpperHPBar_Widget");
	
	widgetComponent->SetupAttachment(widgetOwner->GetMesh());
	widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	widgetComponent->SetWidgetClass(classType);
	widgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	
	UUserWidget* monsterHPBarWidget = widgetComponent->GetUserWidgetObject();
	UHPBar* hpBar = Cast<UHPBar>(monsterHPBarWidget);
	hpBar->SetVisibility(ESlateVisibility::Collapsed);
	
	hpBar->BindStatComponent(widgetOwner->GetStatComponent());

	addWidgetContainer(TEXT("MonsterHPBar"), widgetOwner, hpBar, widgetComponent);
}

void UUIManager::CreateChargingGageBar(ACharacterBase* widgetOwner, float duration) 
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/ChargingGageBar.ChargingGageBar_C'"));
	
	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(
		widgetOwner, UWidgetComponent::StaticClass(), "ChargingGageBar_Widget");
	
	widgetComponent->SetupAttachment(widgetOwner->GetMesh());
	widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	widgetComponent->SetWidgetClass(classType);
	widgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	
	UUserWidget* widgetObject = widgetComponent->GetUserWidgetObject();
	UChargingGageBar* chargingGageBar = Cast<UChargingGageBar>(widgetObject);
	chargingGageBar->SetWidgetComponent(widgetComponent);
	chargingGageBar->StartCharging(duration);

	addWidgetContainer(TEXT("ChargingGageBar"), widgetOwner, chargingGageBar, widgetComponent);
}

void UUIManager::RemoveChargingGageBar(ACharacterBase* widgetComponentOwner)
{
	RemoveWidgetContainers("ChargingGageBar");
}

void UUIManager::SetVisibilityWidgets(const FName& widgetName, UObject* widgetOwner, ESlateVisibility slateVisibility)
{
	if (widgetOwner != nullptr)
	{
		if (m_UIWidgetsWidgetOwnerKey[widgetOwner][widgetName].widget.IsValid())
		{
			m_UIWidgetsWidgetOwnerKey[widgetOwner][widgetName].widget->SetVisibility(slateVisibility);
		}
	}
	else
	{
		for (FWidgetContainer widgetContainer : m_UIWidgetsWidgetNameKey[widgetName])
		{
			if (widgetContainer.widget.IsValid())
			{
				widgetContainer.widget->SetVisibility(slateVisibility);
			}
		}
	}
}

void UUIManager::RemoveWidgetContainers(const FName& widgetName)
{
	for (FWidgetContainer widgetContainer : m_UIWidgetsWidgetNameKey[widgetName])
	{
		if (widgetContainer.widgetComponent.IsValid())
		{
			widgetContainer.widgetComponent->DestroyComponent();
		}
		
		if (widgetContainer.widget.IsValid())
		{
			widgetContainer.widget->RemoveFromParent();
		}
	} 

	m_UIWidgetsWidgetNameKey[widgetName].Empty();
}

void UUIManager::RemoveAllWidgetContainers()
{
	for (TTuple<FName, TArray<FWidgetContainer>> allTypeWidgets : m_UIWidgetsWidgetNameKey)
	{
		RemoveWidgetContainers(allTypeWidgets.Key);
	}

	m_UIWidgetsWidgetNameKey.Empty();
}

UUserWidget* UUIManager::createWidget(const FName& widgetName, const FString& widgetPass, bool bAddToUIWidgets)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, *widgetPass);
	UEnvironmentSettings* environmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), classType));
	environmentSettings->AddToViewport();
	addWidgetContainer(TEXT("EnvironmentSettings"), this, environmentSettings, nullptr);
	
	return nullptr;
}

void UUIManager::addWidgetContainer(const FName& widgetName, UObject* widgetOwner, UUserWidget* widget, UWidgetComponent* widgetComponent)
{
	if (!m_UIWidgetsWidgetNameKey.Contains(widgetName))
	{
		TArray<FWidgetContainer> widgetContainers;
		
		m_UIWidgetsWidgetNameKey.Add(widgetName, widgetContainers);
	}

	m_UIWidgetsWidgetNameKey[widgetName].Add({widget,widgetComponent});

	if (widgetOwner != nullptr)
	{
		if (!m_UIWidgetsWidgetOwnerKey.Contains(widgetOwner))
		{
			TMap<FName, FWidgetContainer> widgetContainer;
			m_UIWidgetsWidgetOwnerKey.Add(widgetOwner, widgetContainer);
		}
	
		m_UIWidgetsWidgetOwnerKey[widgetOwner].Add(widgetName, {widget, widgetComponent});
	}
}
