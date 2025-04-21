// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "UIManager.generated.h"

class ACharacterBase;
class UStatComponent;
class UCombo;
class USkillComponent;

struct FHitInformation;


USTRUCT(Atomic)
struct FWidgetContainer
{
	GENERATED_USTRUCT_BODY()

public:
	TWeakObjectPtr<UUserWidget> widget;
	TWeakObjectPtr<UWidgetComponent> widgetComponent;
};



UCLASS()
class HYOBINSPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void CreateMainPlayerStatusBar(UStatComponent* statComponent, ACharacterBase* widgetOwner);
	void CreateBossStatusBar(UStatComponent* statComponent, ACharacterBase* widgetOwner);
	
	void CreateSkillSlots(USkillComponent* skillComponent, ACharacterBase* widgetOwner);
	void ChangeSkillList();
	
	void OpenEnvironmentSettings();

	UCombo* CreateComboWidget();
	void BindActorToComboWidget(ACharacterBase* hitActor);
	
	void BindActorToDamageWidget(ACharacterBase* hitActor);
	void RenderDamageToScreen(const FHitInformation& hitInfo);
	
	void CreateMonsterHPBar(ACharacterBase* widgetOwner);
	FORCEINLINE void SetIsShowMonsterHPBar(bool bIsShowMonsterHPBar) { m_bIsShowMonsterHPBar = bIsShowMonsterHPBar; }
	FORCEINLINE bool GetIsShowMonsterHPBar() const { return m_bIsShowMonsterHPBar; }
	
	void CreateChargingGageBar(ACharacterBase* widgetOwner, float duration);
	void RemoveChargingGageBar();
	
	void SetVisibilityWidgets(const FName& widgetName, UObject* widgetOwner, ESlateVisibility slateVisibility); // 매개변수 widgetName에 해당하는 모든 위젯을 SetVisibility
	void RemoveWidgetContainers(const FName& widgetName);
	void RemoveAllWidgetContainers();



private:
	UUserWidget* createWidget(const FName& widgetName, const FString& widgetPass, bool bAddToUIWidgets);
	void addWidgetContainer(const FName& widgetName, UObject* widgetOwner, UUserWidget* widget, UWidgetComponent* widgetComponent);
	

private:
	TMap<FName, TArray<FWidgetContainer>> m_UIWidgetsWidgetNameKey;
	TMap<TWeakObjectPtr<UObject>, TMap<FName, FWidgetContainer>> m_UIWidgetsWidgetOwnerKey;
	
	bool m_bIsShowMonsterHPBar;
};



