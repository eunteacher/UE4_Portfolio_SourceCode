// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class My_01 : ModuleRules
{
	public My_01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "CableComponent", "UMG", "Niagara", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent", "Slate", "SlateCore" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "CableComponent" });

        PublicIncludePaths.Add(ModuleDirectory);
        PrivateIncludePaths.Add(ModuleDirectory);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
