// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetProjectGame : ModuleRules
{
	public NetProjectGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
            "NetCore"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[] {
			"OnlineSubsystemNull"
		});

        PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"NetProjectGame",
			//"NetProjectGame/Variant_Platforming",
			//"NetProjectGame/Variant_Platforming/Animation",
			//"NetProjectGame/Variant_Combat",
			//"NetProjectGame/Variant_Combat/AI",
			//"NetProjectGame/Variant_Combat/Animation",
			//"NetProjectGame/Variant_Combat/Gameplay",
			//"NetProjectGame/Variant_Combat/Interfaces",
			//"NetProjectGame/Variant_Combat/UI",
			//"NetProjectGame/Variant_SideScrolling",
			//"NetProjectGame/Variant_SideScrolling/AI",
			//"NetProjectGame/Variant_SideScrolling/Gameplay",
			//"NetProjectGame/Variant_SideScrolling/Interfaces",
			//"NetProjectGame/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
