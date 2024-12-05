// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.generated.h"

class ACharacterBase;
class UStatComponent;
class UHPBar;
class UEnvironmentSettings;
class UCombo;
class UChargingGageBar;
class UWidgetComponent;

//class UMainPlayerSkillComponent;
class USkillComponent;
class UCharging_OnGround;
class USkillSlots;

struct FHitInformation;


UCLASS()
class HYOBINSPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void CreateSkillSlots(USkillComponent* skillComponent);
	void ChangeSkillList();
	
	void OpenEnvironmentSettings() const;

	void BindActorToComboWidget(ACharacterBase* hitActor);
	void CreateComboWidjet();
	
	void BindActorToDamageWidget(ACharacterBase* hitActor);
	void RenderDamageToScreen(const FHitInformation& hitInfo);
	
	void CreateMonsterHPBar(ACharacterBase* actor);
	void ShowMonsterHPBar();
	void HideMonsterHPBar();
	
	void CreateChargingGageBar(ACharacterBase* actor, float duration);
	void RemoveChargingGageBar();
	
	void HideWidgets(const FName& path);
	void ShowWidgets(const FName& path);
	void RemoveWidgets(const FName& path);
	void RemoveAllWidgets(); // ¹Ì¿Ï¼º.

	FORCEINLINE void SetIsShowMonsterHPBar(bool bIsShowMonsterHPBar) { m_bIsShowMonsterHPBar = bIsShowMonsterHPBar; }
	FORCEINLINE bool GetIsShowMonsterHPBar() const { return m_bIsShowMonsterHPBar; }

private:
	void addWidget(TSubclassOf<UUserWidget> widgetClass, UUserWidget* widget);

	
private:
	TMap<TSubclassOf<UUserWidget>, TArray<TObjectPtr<UUserWidget>>> m_UIWidgets;
	
	TObjectPtr<UCombo> m_Combo;
	TWeakObjectPtr<UWidgetComponent> m_ChargingGageBarComponent;
	TWeakObjectPtr<USkillSlots> m_SkillSlots;
	
	TSubclassOf<UUserWidget> m_MonsterHPBarClass;
	TSubclassOf<UUserWidget> m_EnvironmentSettingsClass;
	TSubclassOf<UUserWidget> m_ComboClass;
	TSubclassOf<UUserWidget> m_DamageClass;
	TSubclassOf<UUserWidget> m_ChargingGageBarClass;
	TSubclassOf<UUserWidget> m_SkillSlotsClass;;

	bool m_bIsShowMonsterHPBar;
};
