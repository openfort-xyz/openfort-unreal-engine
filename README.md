![Illustration_01](https://github.com/user-attachments/assets/a1bce733-336b-4e0a-956e-3b33196c4ad2)


<div align="center">
  <h4>
    <a href="https://www.openfort.xyz/">
      Website
    </a>
    <span> | </span>
    <a href="https://www.openfort.xyz/docs/reference/unreal-engine">
      Documentation
    </a>
    <span> | </span>
    <a href="https://www.openfort.xyz/docs/reference/api/introduction">
      API Docs
    </a>
    <span> | </span>
    <a href="https://twitter.com/openfortxyz">
      Twitter
    </a>
  </h4>
</div>

[banner-image]: https://blog-cms.openfort.xyz/uploads/2_42c1d6f4d9.png

# Openfort UE5 SDK

## Installation
1. Since .uasset and .umap files are stored on Git Large File Storage, you must download and install git-lfs from here before cloning the respository
2. Clone the unreal-openfort-sdk repository or download the zip/tarball from one of the versions here
3. Copy the cloned repo into your project's Plugins folder, e.g.: MyGame/Plugins/unreal-openfort-sdk
4. Restart your project (Unreal Editor & IDE), upon restart it should load the unreal-sdk-plugin and should be good to use

## Setup
### Blueprint
See the included sample widgets in All->Plugins->Openfort Content in the Unreal Editor content browser for examples of how to use the plugin in Blueprint. If you don't see a Plugins folder under All you may need to enable Show Plugin Content in the content browser's Settings menu.

### C++
Check out the OpenfortSDK.h file for the C++ OpenfortSDK API. In order to know the list of methods to call for the OpenfortSDK Login Flow and the OpenfortSDK Log out Flow, follow the same sequence of steps as the Blueprint widget examples below.

## Support

| Build Platform | Unreal Engine Version | Windows | MacOS | iOS | Android |
|:--------------:|:---------------------:|---------|-------|-----|---------|
| Windows        | 5.0.3                 | ✅       | ❌     | ❌   | ✅       |
|                | 5.2.1                 | ✅       | ?     | ?   | ✅       |
|           | 5.3              | ❌       | ✅     | ✅   | ✅       |
|           | 5.4                | ❌       | ✅     | ✅   | ✅       |
| macOS          | 5.2.1                 | ❌       | ✅     | ✅   | ✅       |
|           | 5.3                 | ❌       | ✅     | ✅   | ✅       |
|           | 5.4                | ❌       | ✅     | ✅   | ✅       |

### Mac
The plugin currently makes use of the [Web Browser](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/WebBrowser) runtime module which bundles [CEF3](https://www.magpcss.org/ceforum/index.php). Changes to the engine are currently required for modern xcode targets as Unreal Engine does not properly link the framework during packaging. This requires either an engine patch or moving the framework post-build and then codesigning your build again.

In the [CEF3.build.cs](https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/ThirdParty/CEF3/CEF3.build.cs#L102) you need to bypass the `if` check for modern xcode. This can be accomplished by adding `|| true` to the end of the if check on line 102.

Additionally, you need to add `bCompileCEF3 = true;` to your build target.
