// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelBase.h"
#include "LoadingLevel.generated.h"


class ULoadingScreen;
struct FStreamableHandle;



UCLASS()
class HYOBINSPROJECT_API ALoadingLevel : public ALevelBase
{
	GENERATED_BODY()

public:
	ALoadingLevel();

	virtual void BeginPlay() override;

	
	void OnAsyncAssetLoadComplete(FName assetPathName);

	UFUNCTION(BlueprintCallable) 
	void OpenMainLevel();
	
private:
	void asyncLoadAsset(FString assetPath);
	void asyncLoadMainLevel();



private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> m_LoadingScreenClass;
	TObjectPtr<ULoadingScreen> m_LoadingScreen;

	TArray<FString> m_AssetFolderPaths;
	TArray<FString> m_AssetPaths;

	int32 m_AsyncAssetCount;
	int32 m_CompletedAsyncLoadAssetCount;
	TMap<FName, TSharedPtr<FStreamableHandle>> m_LoadHandles;
};
