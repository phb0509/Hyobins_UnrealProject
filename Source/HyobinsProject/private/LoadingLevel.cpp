// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingLevel.h"
#include "AbcFile.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Level/LoadingScreen.h"


ALoadingLevel::ALoadingLevel() :
	m_LoadingScreen(nullptr),
	m_AsyncAssetCount(0),
	m_CompletedAsyncLoadAssetCount(0)
{
	
	// m_AssetFolderPaths.Add("/Game/MainPlayerAsset/Audio");
	// m_AssetFolderPaths.Add("/Game/MainPlayerAsset/Audio2");
	//
	// for (FString& assetFolderPath : m_AssetFolderPaths)
	// {
	// 	FString fileName = "";
	// 	const bool bIsSuccessfullConvertToFilename = FPackageName::TryConvertLongPackageNameToFilename(*assetFolderPath, fileName);
	// 	
	// 	if (bIsSuccessfullConvertToFilename)
	// 	{
	// 		TArray<FString> assetPaths;
	// 		IFileManager::Get().FindFilesRecursive(assetPaths, *fileName, TEXT("*.*"), true, false);
	//
	// 		for (FString& assetPath : assetPaths)
	// 		{
	// 			const bool bIsSuccessfullConvertToLongPackageName = FPackageName::TryConvertFilenameToLongPackageName(*assetPath, assetPath);
	//
	// 			if (bIsSuccessfullConvertToLongPackageName)
	// 			{
	// 				m_AssetPaths.Add(assetPath);
	// 			}
	// 		}
	// 	}
	// }
	//
	// m_AsyncAssetCount = m_AssetPaths.Num();
}

void ALoadingLevel::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ALoadingLevel :: BeginPlay"));
	
	m_LoadingScreen = Cast<ULoadingScreen>(CreateWidget(GetWorld(), m_LoadingScreenClass));
	m_LoadingScreen->AddToViewport();

	//UGameplayStatics::OpenLevel(this, "MainMap");
	asyncLoadMainLevel();
	
	// for (FString& assetFilePath : m_AssetPaths)
	// {
	// 	asyncLoadAsset(assetFilePath);
	// }

}

void ALoadingLevel::asyncLoadAsset(FString assetPath)
{
	FStreamableManager& streamableManager = UAssetManager::GetStreamableManager();
	FSoftObjectPath softObjectPath(assetPath);
	
	TSharedPtr<FStreamableHandle> handle = streamableManager.RequestAsyncLoad(
		softObjectPath,
		FStreamableDelegate::CreateUObject(this, &ALoadingLevel::OnAsyncAssetLoadComplete, FName(*assetPath))
	);

	// 핸들을 저장하여 나중에 상태를 확인할 수 있음
	m_LoadHandles.Add(*assetPath, handle);
}


void ALoadingLevel::OnAsyncAssetLoadComplete(FName assetPathName)
{
	TSharedPtr<FStreamableHandle> handle = m_LoadHandles[assetPathName];
	
	if (handle.IsValid() && handle->HasLoadCompleted())
	{
		UObject* loadedObject = handle->GetLoadedAsset();
	
		if (loadedObject != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s, Asset Load Success!!"), *assetPathName.ToString());

			loadedObject->AddToRoot();
			handle->ReleaseHandle(); //사용한 핸들 닫기
	
			++m_CompletedAsyncLoadAssetCount;
			const float percent = m_CompletedAsyncLoadAssetCount / (static_cast<float>(m_AsyncAssetCount + 1)) * 100.0f;
	
			m_LoadingScreen->SetLoadingPercent(percent);
	
			const FString log = "Count : " + FString::FromInt(m_CompletedAsyncLoadAssetCount) + "   Percent : " + FString::SanitizeFloat(percent);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

			
			if (m_CompletedAsyncLoadAssetCount == m_AsyncAssetCount)
			{
				asyncLoadMainLevel();
			}
		}
	}
}

void ALoadingLevel::asyncLoadMainLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("ALoadingLevel :: asyncLoadMainLevel"));
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "OpenMainLevel";
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 1;
	
	UGameplayStatics::LoadStreamLevel(this, "MainMap", true, true, LatentInfo);
}

void ALoadingLevel::OpenMainLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("ALoadingLevel :: OpenMainLevel"));

	UGameplayStatics::UnloadStreamLevel(this, "LoadingLevel", FLatentActionInfo(), false);

	UGameplayStatics::OpenLevel(this, "MainMap");
}


