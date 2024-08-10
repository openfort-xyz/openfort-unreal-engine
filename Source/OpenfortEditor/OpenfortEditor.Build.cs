// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OpenfortEditor : ModuleRules
{
    public OpenfortEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_5_1_OR_LATER
			IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif

        PublicIncludePaths.AddRange(
            new string[]
            {
				// ... add public include paths required here ...
			}
        );


        PrivateIncludePaths.AddRange(
            new string[]
            {
				// ... add other private include paths required here ...
			}
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				// ... add other public dependencies that you statically link with here ...
			}
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Json",
                "JsonUtilities",
                "UMG",
                "Projects",
                "Settings",
                "AssetTools",
                "UnrealEd",
                "Openfort",
				// ... add private dependencies that you statically link with here ...
			}
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
        );

        if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.Linux)
        {
            // RuntimeDependencies.Add("$(PluginDir)/Content/index.html");
        }
    }
}