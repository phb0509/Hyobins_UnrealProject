// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HyobinsProjectTarget : TargetRules
{
	public HyobinsProjectTarget( TargetInfo Target) : base(Target)
	{
		// Windows SDK ���� ����
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// ���ϴ� SDK ���� ���� 
			WindowsPlatform.WindowsSdkVersion = "10.0.18362.0";
		}
		
		CppStandard = CppStandardVersion.Cpp17;
		
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "HyobinsProject", "EditorModule" } );
	}
}
