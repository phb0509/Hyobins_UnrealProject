// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/UIManager.h"
#include "CharacterBase/CharacterBase.h"
#include "Utility/CustomStructs.h"
#include "Components/WidgetComponent.h"
#include "UI/System/EnvironmentSettings.h"
#include "UI/System/Combo.h"
#include "UI/System/Damage.h"
#include "UI/MainPlayer/ChargingGageBar.h"
#include "UI/MainPlayer/SkillSlots.h"
#include "UI/MainPlayer/MainPlayerStatusBar.h"
#include "UI/Monster/BossStatusBar.h"
#include "UI/System/LockOn.h"


void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	m_bIsShowMonsterHPBar = true;
	m_HPBarReserveSize = 100;
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();

	RemoveAllWidgetContainers();
	m_WhiteBlinkHPBars.Empty();
}

void UUIManager::CreateMainPlayerStatusBar(UStatComponent* statComponent, ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/StatusBar.StatusBar_C'"));
	check(classType != nullptr);
	
	UMainPlayerStatusBar* mainPlayerStatusBar = Cast<UMainPlayerStatusBar>(CreateWidget(GetWorld(), classType));
	check(mainPlayerStatusBar != nullptr);
	
	mainPlayerStatusBar->BindStatComponent(statComponent);
	mainPlayerStatusBar->AddToViewport();
	
	addWidgetContainer(TEXT("MainPlayerStatusBar"), mainPlayerStatusBar, nullptr);
}

void UUIManager::CreateBossStatusBar(UStatComponent* statComponent, ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/BossStatusBar.BossStatusBar_C'"));
	check(classType != nullptr);
	
	UBossStatusBar* bossStatusBar = Cast<UBossStatusBar>(CreateWidget(GetWorld(), classType));
	check(bossStatusBar != nullptr);
	
	bossStatusBar->BindStatComponent(statComponent);
	bossStatusBar->AddToViewport();
	
	addWidgetContainer(TEXT("BossStatusBar"), bossStatusBar, nullptr);
}

void UUIManager::CreateSkillSlots(USkillComponent* skillComponent, ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/SkillSlots.SkillSlots_C'"));
	check(classType != nullptr);
	
	USkillSlots* skillSlots = Cast<USkillSlots>(CreateWidget(GetWorld(), classType));
	check(skillSlots != nullptr);
	
	skillSlots->AddToViewport();
	
	addWidgetContainer(TEXT("SkillSlots"), skillSlots, nullptr);
	
	skillSlots->CreateSkillList(skillComponent);
}

void UUIManager::ChangeSkillList()
{
	USkillSlots* skillSlots = Cast<USkillSlots>(m_UIWidgetsWidgetNameKey["SkillSlots"][0].widget);
	check(skillSlots != nullptr);
	
	skillSlots->ChangeSkillSlots();
}

void UUIManager::CreateLockOn(AActor* target)
{
	if (target == nullptr)
	{
		return;
	}
	
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/LockOn.LockOn_C'"));
	check(classType != nullptr);

	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(target, UWidgetComponent::StaticClass(), "LockOn_Widget");
	check(widgetComponent != nullptr);

	widgetComponent->SetupAttachment(target->GetRootComponent());
	widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	widgetComponent->SetWidgetClass(classType);
	widgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));

	UUserWidget* widget = widgetComponent->GetUserWidgetObject();
	check(widget != nullptr);
	
	ULockOn* lockOnWidget = Cast<ULockOn>(widget);
	check(lockOnWidget != nullptr);
	
	lockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

	addWidgetContainer(TEXT("LockOn"), lockOnWidget, widgetComponent);
}

void UUIManager::RenderLockOnToScreen(AActor* target)
{
	if (!IsWidgetCreated(TEXT("LockOn")))
	{
		CreateLockOn(target);
		
		return;
	}
	
	UWidgetComponent* widgetComponent = Cast<UWidgetComponent>(m_UIWidgetsWidgetNameKey["LockOn"][0].widgetComponent);
	check(widgetComponent != nullptr);
	
	widgetComponent->AttachToComponent(target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
	ULockOn* lockOnWidget = Cast<ULockOn>(widgetComponent->GetUserWidgetObject());
	check(lockOnWidget != nullptr);
	
	lockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUIManager::OpenEnvironmentSettings()
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/EnvironmentSettings.EnvironmentSettings_C'"));
	check(classType != nullptr);
	
	UEnvironmentSettings* environmentSettings = Cast<UEnvironmentSettings>(CreateWidget(GetWorld(), classType));
	check(environmentSettings != nullptr);
	
	environmentSettings->AddToViewport();
	environmentSettings->Open();

	addWidgetContainer(TEXT("EnvironmentSettings"), environmentSettings, nullptr);
}

void UUIManager::CreateComboWidget()
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Combo.Combo_C'"));
	check(classType != nullptr);
	
	UCombo* combo = Cast<UCombo>(CreateWidget(GetWorld(), classType));
	check(combo != nullptr);
	
	combo->AddToViewport();
	combo->SetVisibility(ESlateVisibility::Collapsed);
	
	addWidgetContainer(TEXT("Combo"), combo, nullptr);
	
	m_Combo = combo;
}

void UUIManager::UpdateComboCount()
{
	if (!m_Combo.IsValid())
	{
		CreateComboWidget();
	}
	
	m_Combo->UpdateComboCount();
}

void UUIManager::BindActorToDamageWidget(ACharacterBase* hitActor)
{
	hitActor->OnTakeDamage.AddUObject(this, &UUIManager::RenderDamageToScreen);
}

void UUIManager::RenderDamageToScreen(const FHitInformation& hitInfo)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/System/Damage.Damage_C'"));
	check(classType != nullptr);
	
	UDamage* damageWidget = Cast<UDamage>(CreateWidget(GetWorld(), classType));
	check(damageWidget != nullptr);
	
	damageWidget->AddToViewport();
	
	FVector2D screenPosition;
	GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(hitInfo.hitActor->GetActorLocation(), screenPosition);
	screenPosition.Y -= 100.0f;
	
	damageWidget->SetPositionInViewport(screenPosition);
	damageWidget->SetDamage(hitInfo.finalDamage);
	damageWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUIManager::ReserveMonsterHPBars()
{
	m_WhiteBlinkHPBars.Reserve(m_HPBarReserveSize);
}

void UUIManager::EmptyMonsterHPBars()
{
	m_WhiteBlinkHPBars.Empty();
}

void UUIManager::CreateMonsterHPBar(ACharacterBase* widgetOwner)
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/Monster/NewHealth/BP_MonsterHPBar.BP_MonsterHPBar_C'"));
	check(classType != nullptr);
	
	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(
		widgetOwner, UWidgetComponent::StaticClass(), "UpperHPBar_Widget");
	
	check(widgetComponent != nullptr);
	
	widgetComponent->SetupAttachment(widgetOwner->GetMesh());
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	widgetComponent->SetWidgetClass(classType);
	widgetComponent->SetRelativeLocation(FVector(0,0,250.0f));
	
	UUserWidget* monsterHPBarWidget = widgetComponent->GetUserWidgetObject();
	check(monsterHPBarWidget != nullptr);
	
	UMonsterHPBar* hpBar = Cast<UMonsterHPBar>(monsterHPBarWidget);
	check(hpBar != nullptr);
	
	hpBar->SetVisibility(ESlateVisibility::Collapsed);
	hpBar->BindStatComponent(widgetOwner->GetStatComponent());

	addWidgetContainer(TEXT("MonsterHPBar"), hpBar, widgetComponent);

	++m_HPBarReserveSize;
}

void UUIManager::RegistWhiteBlinkTick(UMonsterHPBar* hpBar)
{
	if (IsValid(hpBar)|| m_WhiteBlinkHPBars.Contains(hpBar))
	{
		return;
	}
	
	m_WhiteBlinkHPBars.Add(hpBar); //
	hpBar->InitWhiteBlink(); 

	if (!GetWorld()->GetTimerManager().IsTimerActive(m_WhiteBlinkSharedTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			m_WhiteBlinkSharedTimerHandle,
			this,
			&UUIManager::UpdateHPBarWhiteBlink,
			0.03f,
			true
		);
	}
}

void UUIManager::UpdateHPBarWhiteBlink()
{
	for (int32 i = m_WhiteBlinkHPBars.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<UMonsterHPBar> hpBar = m_WhiteBlinkHPBars[i].Get();
		
		if (!hpBar.IsValid())
		{
			m_WhiteBlinkHPBars.RemoveAtSwap(i,1,false);
			
			continue;
		}
		
		if (!hpBar->WhiteBlinkTick())
		{
			m_WhiteBlinkHPBars.RemoveAtSwap(i,1,false);
		}
	}
	
	if (m_WhiteBlinkHPBars.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_WhiteBlinkSharedTimerHandle);
	}
}

void UUIManager::CreateChargingGageBar(ACharacterBase* widgetOwner, float duration) 
{
	TSubclassOf<UUserWidget> classType = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/UI/MainPlayer/ChargingGageBar.ChargingGageBar_C'"));
	check(classType != nullptr);
	
	UWidgetComponent* widgetComponent = NewObject<UWidgetComponent>(
		widgetOwner, UWidgetComponent::StaticClass(), "ChargingGageBar_Widget");
	check(widgetComponent != nullptr);
	
	widgetComponent->SetupAttachment(widgetOwner->GetMesh());
	widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	widgetComponent->RegisterComponentWithWorld(GetWorld());
	widgetComponent->SetWidgetClass(classType);
	widgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
	
	UUserWidget* widgetObject = widgetComponent->GetUserWidgetObject();
	check(widgetObject != nullptr);
	
	UChargingGageBar* chargingGageBar = Cast<UChargingGageBar>(widgetObject);
	check(chargingGageBar != nullptr);
	
	chargingGageBar->SetWidgetComponent(widgetComponent);
	chargingGageBar->StartCharging(duration);

	addWidgetContainer(TEXT("ChargingGageBar"), chargingGageBar, widgetComponent);
}

void UUIManager::SetVisibilityWidgets(const FName& widgetName, ESlateVisibility slateVisibility)
{
	if (!m_UIWidgetsWidgetNameKey.Contains(widgetName))
	{
		return;
	}
	
	for (FWidgetContainer widgetContainer : m_UIWidgetsWidgetNameKey[widgetName])
	{
		if (widgetContainer.widget.IsValid())
		{
			widgetContainer.widget->SetVisibility(slateVisibility);
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

bool UUIManager::IsWidgetCreated(const FName& widgetName)
{
	if (!m_UIWidgetsWidgetNameKey.Contains(widgetName))
	{
		return false;
	}
	
	for (FWidgetContainer widgetContainer : m_UIWidgetsWidgetNameKey[widgetName])
	{
		if (widgetContainer.widgetComponent.IsValid())
		{
			return true;
		}
		
		if (widgetContainer.widget.IsValid())
		{
			return true;
		}
	}

	return false;
}

void UUIManager::addWidgetContainer(const FName& widgetName, UUserWidget* widget, UWidgetComponent* widgetComponent)
{
	if (!m_UIWidgetsWidgetNameKey.Contains(widgetName))
	{
		TArray<FWidgetContainer> widgetContainers;
		
		m_UIWidgetsWidgetNameKey.Add(widgetName, widgetContainers);
	}

	m_UIWidgetsWidgetNameKey[widgetName].Add({widget,widgetComponent});
}
