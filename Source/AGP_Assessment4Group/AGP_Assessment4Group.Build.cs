// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AGP_Assessment4Group : ModuleRules
{
	public AGP_Assessment4Group(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
