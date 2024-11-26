// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "Utility/CharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/Skill/Charging_OnGround.h"

#include "Utility/CustomStructs.h"
#include "UI/HPBar.h"
#include "UI/System/EnvironmentSettings.h"
#include "UI/System/Combo.h"
#include "UI/System/Damage.h"
#include "UI/ChargingGageBar.h"




void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_MonsterHPBarClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/HPBar.HPBar_C'"));
	m_EnvironmentSettingsClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/EnvironmentSettings.EnvironmentSettings_C'"));
	m_ComboClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Combo.Combo_C'"));
	m_DamageClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Damage.Damage_C'"));
	m_ChargingGageBarClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/ChargingGageBar.ChargingGageBar_C'"));
	
	m_bIsShowMonsterHPBar = true;
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();

	RemoveAllWidgets();
}

void UUIManager::OpenEnvironmentSettings() const
{
	UEnvironmentSettings* environmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), m_EnvironmentSettingsClass));
	environmentSettings->AddToViewport();
	environmentSettings->Open();
}

void UUIManager::BindActorToComboWidget(ACharacterBase* hitActor)
{
	if (m_Combo == nullptr)
	{
		CreateComboWidjet();
	}

	if (m_Combo != nullptr)
	{
		hitActor->OnTakeDamage.AddUObject(m_Combo.Get(), &UCombo::UpdateComboCount);
	}
}

void UUIManager::CreateComboWidjet()
{
	if (m_Combo != nullptr) return;
	
	m_Combo = Cast<UCombo>(CreateWidget(GetWorld(), m_ComboClass));
	m_Combo->AddToViewport();
	m_Combo->SetVisibility(ESlateVisibility::Collapsed);
}


void UUIManager::BindActorToDamageWidget(ACharacterBase* hitActor)
{
	hitActor->OnTakeDamage.AddUObject(this, &UUIManager::RenderDamageToScreen);
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
}

void UUIManager::CreateMonsterHPBar(ACharacterBase* actor)
{
	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(
		actor, UWidgetComponent::StaticClass(), "UpperHPBar_Widget");
	
	widgetComponent->SetupAttachment(actor->GetMesh());
	widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	widgetComponent->SetWidgetClass(m_MonsterHPBarClass);
	widgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	
	UUserWidget* monsterHPBarWidget = widgetComponent->GetUserWidgetObject();
	UHPBar* hpBar = Cast<UHPBar>(monsterHPBarWidget);
	
	hpBar->BindStatComponent(actor->GetStatComponent());

	addWidget(m_MonsterHPBarClass, monsterHPBarWidget);
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

void UUIManager::BindChargingSkillToChargingGageBar(UCharging_OnGround* chargingSkill_OnGround)
{
	if (chargingSkill_OnGround != nullptr)
	{
		chargingSkill_OnGround->m_OnChargingDelegate.BindUObject(this, &UUIManager::CreateChargingGageBar);
		chargingSkill_OnGround->m_OnStopChargingDeleagte.BindUObject(this, &UUIManager::RemoveChargingGageBar);
	}
}

void UUIManager::CreateChargingGageBar(ACharacterBase* actor, float duration) 
{
	m_ChargingGageBarComponent = NewObject<UWidgetComponent>(
		actor, UWidgetComponent::StaticClass(), "ChargingGageBar_Widget");
	
	m_ChargingGageBarComponent->SetupAttachment(actor->GetMesh());
	m_ChargingGageBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	m_ChargingGageBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	m_ChargingGageBarComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	m_ChargingGageBarComponent->RegisterComponentWithWorld(GetWorld());
	m_ChargingGageBarComponent->SetWidgetClass(m_ChargingGageBarClass);
	m_ChargingGageBarComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	
	UUserWidget* widgetObject = m_ChargingGageBarComponent->GetUserWidgetObject();
	
	m_ChargingGageBar = Cast<UChargingGageBar>(widgetObject);
	m_ChargingGageBar->SetWidgetComponent(m_ChargingGageBarComponent.Get());
	m_ChargingGageBar->Play(duration);

	// 애니메이션재생완료시 위젯에서 DestoryComponent 호출.
}

void UUIManager::RemoveChargingGageBar()
{
	if (m_ChargingGageBarComponent.IsValid())
	{
		m_ChargingGageBarComponent->DestroyComponent();
	}
}


void UUIManager::HideWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());

	if (widgetClass != nullptr)
	{
		for (const TWeakObjectPtr<UUserWidget> widget : m_UIWidgets[widgetClass])
		{
			if (widget.IsValid())
			{
				widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUIManager::ShowWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());

	if (widgetClass != nullptr)
	{
		for (auto widget : m_UIWidgets[widgetClass])
        	{
        		if (IsValid(widget))
        		{
        			widget->SetVisibility(ESlateVisibility::HitTestInvisible);
        		}
        	}
	}
}

void UUIManager::RemoveWidgets(const FName& path)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, *path.ToString());
	
	if (widgetClass != nullptr)
	{
		if (m_UIWidgets.Contains(widgetClass))
		{
			for (auto widget : m_UIWidgets[widgetClass])
			{
				if (IsValid(widget))
				{
					widget->RemoveFromParent();
				}
			}
		}
		
		m_UIWidgets[widgetClass].Empty();
	}
}

void UUIManager::RemoveAllWidgets()
{
	for (auto iter : m_UIWidgets)
	{
		TSubclassOf<UUserWidget> classType = iter.Key;

		for (auto widget : iter.Value)
		{
			if (IsValid(widget))
			{
				widget->RemoveFromParent();
			}
		}
	}

	m_UIWidgets.Empty();
}

void UUIManager::addWidget(TSubclassOf<UUserWidget> widgetClass, UUserWidget* widget)
{
	if (IsValid(widgetClass))
	{
		if (!m_UIWidgets.Contains(widgetClass))
		{
			TArray<TObjectPtr<UUserWidget>> widgetArray;
			m_UIWidgets.Add(widgetClass, widgetArray);
		}

		if (IsValid(widget))
		{
			m_UIWidgets[widgetClass].Add(widget);
		}
	}
}
