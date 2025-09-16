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
class UMonsterHPBar;

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

	void CreateLockOn(AActor* target);
	void RenderLockOnToScreen(AActor* target);
	
	void OpenEnvironmentSettings();

	void CreateComboWidget();
	void UpdateComboCount();
	
	void BindActorToDamageWidget(ACharacterBase* hitActor);
	void RenderDamageToScreen(const FHitInformation& hitInfo);
	
	void ReserveMonsterHPBars();
	void EmptyMonsterHPBars();

	// HPBar
	void CreateMonsterHPBar(ACharacterBase* widgetOwner);
	FORCEINLINE void SetIsShowMonsterHPBar(bool bIsShowMonsterHPBar) { m_bIsShowMonsterHPBar = bIsShowMonsterHPBar; }
	FORCEINLINE bool IsShowMonsterHPBar() const { return m_bIsShowMonsterHPBar; }
	void RegistWhiteBlinkTick(UMonsterHPBar* hpBar);
	void UpdateHPBarWhiteBlink();

	// Player ChargingGageBar
	void CreateChargingGageBar(ACharacterBase* widgetOwner, float duration);
	void SetVisibilityWidgets(const FName& widgetName, ESlateVisibility slateVisibility); // 매개변수 widgetName에 해당하는 모든 위젯을 SetVisibility
	void RemoveWidgetContainers(const FName& widgetName);
	void RemoveAllWidgetContainers();
	bool IsWidgetCreated(const FName& widgetName);


private:
	void addWidgetContainer(const FName& widgetName,UUserWidget* widget, UWidgetComponent* widgetComponent);
	

private:
	TMap<FName, TArray<FWidgetContainer>> m_UIWidgetsWidgetNameKey;
	
	bool m_bIsShowMonsterHPBar;
	
    TArray<TWeakObjectPtr<UMonsterHPBar>> m_WhiteBlinkHPBars;           
    FTimerHandle m_WhiteBlinkSharedTimerHandle;         
    int32  m_HPBarReserveSize;

	TWeakObjectPtr<UCombo> m_Combo;
};



