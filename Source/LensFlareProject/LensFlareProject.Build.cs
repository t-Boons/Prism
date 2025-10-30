// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LensFlareProject : ModuleRules
{
	public LensFlareProject(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LensFlareProject",
			"LensFlareProject/Variant_Horror",
			"LensFlareProject/Variant_Horror/UI",
			"LensFlareProject/Variant_Shooter",
			"LensFlareProject/Variant_Shooter/AI",
			"LensFlareProject/Variant_Shooter/UI",
			"LensFlareProject/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
