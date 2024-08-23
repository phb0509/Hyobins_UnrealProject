// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UStatComponent;


UCLASS()
class HYOBINSPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override {}
	virtual void Deinitialize() override {}

	void CreateHPBarComponent(AActor* actor, UStatComponent* const statComponent, USceneComponent* mesh, const FName& subObjectName, const FString& assetPath, const FVector& relativeLocation, const FVector2D& drawSize);
	void HideWidgets(const FName& path);
	void ShowWidgets(const FName& path);
	void ClearWidgets(const FName& path);
	void ClearAllWidgets(); // ¹Ì¿Ï¼º.
	

public:
	TMap<TSubclassOf<UUserWidget>, TArray<UUserWidget*>> m_UIWidgets;
};
