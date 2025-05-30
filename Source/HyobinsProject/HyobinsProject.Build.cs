// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HyobinsProject : ModuleRules
{
	public HyobinsProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem",
			"AIModule", "GameplayTasks", "UMG", "MotionWarping", "EnhancedInput", "Niagara", "GameplayCameras"});

		PrivateDependencyModuleNames.AddRange(new string[] { "AlembicLibrary" });

		// Uncomment if you are using Slate UI
		 PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PropertyEditor" });
		
		 CppStandard = CppStandardVersion.Cpp17;
		 //PublicDefinitions.Add("__has_feature(x)=0");
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
