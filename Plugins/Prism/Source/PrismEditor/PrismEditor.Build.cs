// Copyright tBoons 2025

using UnrealBuildTool;

public class PrismEditor : ModuleRules
{
    public PrismEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "EditorStyle",
            "UnrealEd",
            "AssetTools",
            "Projects",
            "ToolMenus",
            "Prism"
        });
    }
}