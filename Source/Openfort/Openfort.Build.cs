// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class Openfort : ModuleRules
{
    public Openfort(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        
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
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "JsonUtilities",
                "HTTP",
                "UMG",
                "OnlineSubsystem",
                "WebBrowser",
                "WebBrowserWidget"
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "EngineSettings",
                "Json",
                "Projects"
				// ... add private dependencies that you statically link with here ...
			}
        );

        PublicDefinitions.Add("USING_BUNDLED_CEF=1");

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