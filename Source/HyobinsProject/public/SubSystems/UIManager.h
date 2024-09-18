// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UStatComponent;
class UHPBar;
class UEnvironmentSettings;
class UCombo;

UCLASS()
class HYOBINSPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void CreateEnvironmentSettings();
	void ShowEnvironmentSettings();
	void HideEnvironmentSettings();
	FORCEINLINE UEnvironmentSettings* GetEnvironmentSettingsWidjet();

	void CreateComboWidjet();
	void BindStatComponentToComboWidget(UStatComponent* const statComponent);

	void RenderDamageToScreen(const FVector& worldPosition, float damage);
	
	void CreateMonsterHPBar(AActor* actor, UStatComponent* const statComponent, USceneComponent* mesh, const FName& subObjectName, const FVector& relativeLocation, const FVector2D& drawSize);
	void ShowMonsterHPBar();
	void HideMonsterHPBar();
	
	void HideWidgets(const FName& path);
	void ShowWidgets(const FName& path);
	void ClearWidgets(const FName& path);
	void ClearAllWidgets(); // ¹Ì¿Ï¼º.


private:
	//TMap<TSubclassOf<UUserWidget>, TArray<UUserWidget*>> m_UIWidgets;
	TMap<TSubclassOf<UUserWidget>, TArray<TWeakObjectPtr<UUserWidget>>> m_UIWidgets;

	TObjectPtr<UEnvironmentSettings> m_EnvironmentSettings;
	TObjectPtr<UCombo> m_Combo;
	
	TSubclassOf<UUserWidget> m_MonsterHPBarClass;
	TSubclassOf<UUserWidget> m_EnvironmentSettingsClass;
	TSubclassOf<UUserWidget> m_ComboClass;
	TSubclassOf<UUserWidget> m_DamageClass;
};
