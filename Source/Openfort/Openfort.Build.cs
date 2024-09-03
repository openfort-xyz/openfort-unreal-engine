// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class Openfort : ModuleRules
{
    public Openfort(ReadOnlyTargetRules Target) : base(Target)
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
                "JsonUtilities",
                "HTTP"
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
                "UMG",
                "Projects", 
				// ... add private dependencies that you statically link with here ...
			}
        );

#if UE_5_0_OR_LATER
			PublicDependencyModuleNames.Add("WebBrowserWidget");
			PrivateDependencyModuleNames.Add("WebBrowser");
			PublicDefinitions.Add("USING_BUNDLED_CEF=1");
			PublicDefinitions.Add("USING_BLUI_CEF=0");
#else
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateDependencyModuleNames.Add("Blu");
            PublicDefinitions.Add("USING_BLUI_CEF=1");
        }
        else
        {
            PublicDefinitions.Add("USING_BLUI_CEF=0");
        }

        PublicDefinitions.Add("USING_BUNDLED_CEF=0");
#endif

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
        );

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Openfort_UPL_Android.xml"));
        }

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private/Openfort", "IOS") });

            PublicFrameworks.AddRange(
                new string[]
                {
                    "Foundation",
                    "AuthenticationServices"
                }
            );
        }

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private/Openfort", "Mac") });

            PublicFrameworks.AddRange(
                new string[]
                {
                    "Foundation",
                    "AuthenticationServices"
                }
            );
        }
    }
}