// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorModule.h"
#include "DetailCustomization_SetFSMServiceNode.h"
#include "Modules/ModuleManager.h"
#include "Monster/BTS_SetFSMState.h"


IMPLEMENT_MODULE(FEditorModule, EditorModule);

void FEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("FEditorModule :: StartupModule"));
	
	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	propertyModule.RegisterCustomClassLayout(
		FName(*UBTS_SetFSMState::StaticClass()->GetName()),
		FOnGetDetailCustomizationInstance::CreateStatic(&DetailCustomization_SetFSMServiceNode::MakeInstance));
}

void FEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("FEditorModule :: ShutdownModule"));
	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	propertyModule.UnregisterCustomClassLayout(
		FName(*UBTS_SetFSMState::StaticClass()->GetName()));
}

