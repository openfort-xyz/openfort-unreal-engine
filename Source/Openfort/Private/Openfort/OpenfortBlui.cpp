// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenfortBlui.h"
#include "Openfort/Assets/OpenfortSDKResource.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "Openfort/OpenfortJSConnector.h"
#include <utility>

#if USING_BLUI_CEF
#include "BluEye.h"
#endif

UOpenfortBlui::UOpenfortBlui()
{
  OPENFORT_LOG_FUNCSIG
#if USING_BLUI_CEF
  if (!BluEyePtr)
  {
    OPENFORT_LOG("Creating BluEye")
    BluEyePtr = NewObject<UBluEye>(this, "BluEye");
  }
#endif

  JSConnector = NewObject<UOpenfortJSConnector>(this, "JSConnector");
  JSConnector->ExecuteJs = UOpenfortJSConnector::FOnExecuteJsDelegate::CreateUObject(this, &UOpenfortBlui::ExecuteJS);
}

#if USING_BLUI_CEF
UBluEye *UOpenfortBlui::GetBluEye() const { return Cast<UBluEye>(BluEyePtr); }
#endif

void UOpenfortBlui::OnLogEvent(const FString &LogText)
{
  OPENFORT_LOG_FUNC("LogEvent: %s", *LogText);
}

void UOpenfortBlui::WorldTickStart(UWorld *World, ELevelTick LevelTick, float X)
{
#if USING_BLUI_CEF
  if (!GetBluEye()->IsBrowserLoading() && !bLoadedIndexJS)
  {
    const FSoftObjectPath AssetRef(
        TEXT("/Script/Openfort.OpenfortSDKResource'/Openfort/PackagedResources/"
             "index.index'"));

    OPENFORT_LOG("Browser loaded");
    bLoadedIndexJS = true;
    if (UObject *LoadedAsset = AssetRef.TryLoad())
    {
      if (const auto Resource = Cast<UOpenfortSDKResource>(LoadedAsset))
      {
        GetBluEye()->ExecuteJS(Resource->Js);
        OPENFORT_VERBOSE("Loaded index.js")
      }
      else
      {
        OPENFORT_ERR("Error in loading index.js")
      }
    }
    else
    {
      OPENFORT_ERR("Error in loading index.js")
    }
  }
#endif
}

void UOpenfortBlui::BeginDestroy()
{
  OPENFORT_LOG_FUNCSIG
#if USING_BLUI_CEF
  if (GetBluEye())
  {
    GetBluEye()->CloseBrowser();
  }
  BluEyePtr = nullptr;
#endif
  Super::BeginDestroy();
}

void UOpenfortBlui::OnScriptEvent(const FString &EventName, const FString &EventMessage)
{
  OPENFORT_LOG_FUNC("EventName: %s, EventMessage: %s", *EventName, *EventMessage);
  JSConnector->SendToGame(EventMessage);
}

TWeakObjectPtr<UOpenfortJSConnector> UOpenfortBlui::GetJSConnector() const
{
  return JSConnector;
}

void UOpenfortBlui::ExecuteJS(const FString &ScriptText) const
{
#if USING_BLUI_CEF
  if (GetBluEye())
  {
    GetBluEye()->ExecuteJS(ScriptText);
  }
#endif
}

void UOpenfortBlui::Init()
{
  OPENFORT_LOG_FUNCSIG

#if USING_BLUI_CEF
  // Todo: Add comment here why GetBluEye
  UBluEye *BluEye = GetBluEye();

  BluEye->LogEventEmitter.AddUniqueDynamic(this, &UOpenfortBlui::OnLogEvent);
  BluEye->ScriptEventEmitter.AddUniqueDynamic(this, &UOpenfortBlui::OnScriptEvent);

  BluEye->bEnabled = true;
  OPENFORT_LOG("Events subscribed")

  BluEye->Init();
  OPENFORT_LOG("BluEye Initialised")

  FSoftObjectPath AssetRef(TEXT("/Script/Openfort.OpenfortSDKResource'/Openfort/"
                                "PackagedResources/index.index'"));
  if (UObject *LoadedAsset = AssetRef.TryLoad())
  {
    if (auto Resource = Cast<UOpenfortSDKResource>(LoadedAsset))
    {
      // We're attempting to replicate the process that Unreal's WebBrowser
      // widget uses to load a page from a string. Unfortunately this doesn't
      // work correctly, but it still solves our issue. LocalStorage can't be
      // accessed from about:blank or data URIs, so we still need to load a
      // page.  Despite this failing to load our custom html, this approach
      // still allows us to access LocalStorage and use the game bridge.  If
      // there was more time in the future it would probably be worth
      // investigating the issues here.

      // PostData
      CefRefPtr<CefPostData> PostData = CefPostData::Create();
      CefRefPtr<CefPostDataElement> Element = CefPostDataElement::Create();
      FTCHARToUTF8 UTF8String(TEXT(
          "<!doctype html><html lang='en'><head><meta "
          "charset='utf-8'><title>GameSDK "
          "Bridge</title></head><body><h1>Bridge Running</h1></body></html>"));
      Element->SetToBytes(UTF8String.Length(), UTF8String.Get());
      PostData->AddElement(Element);

      CefRequest::HeaderMap HeaderMap;
      HeaderMap.insert(std::pair<CefString, CefString>(
          TCHAR_TO_WCHAR(TEXT("Content-Type")), "html"));

      const FString CustomContentMethod(TEXT("X-GET-CUSTOM-CONTENT"));

      const auto Request = CefRequest::Create();
      Request->Set("file:///Openfort/index.html", *CustomContentMethod,
                   PostData, HeaderMap);

      GetBluEye()->Browser->GetMainFrame()->LoadRequest(Request);
      OPENFORT_VERBOSE("LoadRequest'ed for Index.html")

      WorldTickHandle = FWorldDelegates::OnWorldTickStart.AddUObject(
          this, &UOpenfortBlui::WorldTickStart);
      OPENFORT_LOG("Waiting for Browser to load...");
    }
  }
  else
  {
    OPENFORT_ERR("Failed to load Openfort bridge asset.")
  }

  // Do this after the the page is given to the browser and being loaded...
  JSConnector->Init(!BluEye->IsBrowserLoading());
#endif
}

#if USING_BLUI_CEF
void UOpenfortBlui::StopBluiEventLoop()
{
  if (UBluEye *BluEye = GetBluEye())
  {
    BluEye->SetShouldTickEventLoop(false);
  }
}
#endif
