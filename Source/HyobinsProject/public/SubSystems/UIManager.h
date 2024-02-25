// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"


UCLASS()
class HYOBINSPROJECT_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override {}
	virtual void Deinitialize() override {}

	void CreateHPBarComponent(class AActor* actor, class USceneComponent* mesh, const FName& subObjectName, const FString& assetPath, const FVector& relativeLocation, const FVector2D& drawSize);
	void HideWidgets(const FName& path);
	void ShowWidgets(const FName& path);
	void ClearWidgets(const FName& path);
	void ClearAllWidgets(); // ¹Ì¿Ï¼º.
	

public:
	TMap<TSubclassOf<class UUserWidget>, TArray<class UUserWidget*>> m_UIWidgets;
};
