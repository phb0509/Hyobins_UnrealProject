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

	void CreateUpperHPBar(class AActor* actor, class USceneComponent* mesh, FName subObjectName, FString assetPath, FVector relativeLocation, FVector2D drawSize);

private:
	TMap<TWeakObjectPtr<class AActor>, TMap<TSubclassOf<class UUserWidget>, TWeakObjectPtr<class UUserWidget>>> m_UIWidgets;
};
