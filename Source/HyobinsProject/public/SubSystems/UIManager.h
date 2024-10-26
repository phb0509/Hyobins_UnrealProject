// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class ACharacterBase;
class UStatComponent;
class UHPBar;
class UEnvironmentSettings;
class UCombo;

struct FHitInformation;

UCLASS()
class HYOBINSPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void OpenEnvironmentSettings() const;

	void CreateComboWidjet();
	void BindActorToComboWidget(ACharacterBase* const hitActor);

	void RenderDamageToScreen(const FHitInformation& hitInfo);
	void BindActorToDamageWidget(ACharacterBase* const hitActor);
	
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
	
	TObjectPtr<UCombo> m_Combo;
	
	TSubclassOf<UUserWidget> m_MonsterHPBarClass;
	TSubclassOf<UUserWidget> m_EnvironmentSettingsClass;
	TSubclassOf<UUserWidget> m_ComboClass;
	TSubclassOf<UUserWidget> m_DamageClass;
	
};
