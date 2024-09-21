// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenfortBrowserWidget.h"

#include "Openfort/Misc/OpenfortLogging.h"
#include "Openfort/OpenfortJSConnector.h"
#if USING_BUNDLED_CEF
#include "WebBrowser.h"
#endif
#include "Openfort/Assets/OpenfortSDKResource.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Interfaces/IPluginManager.h"

UOpenfortBrowserWidget::UOpenfortBrowserWidget()
{
	OPENFORT_LOG_FUNCSIG

	JSConnector = NewObject<UOpenfortJSConnector>(this, "JSConnector");
	JSConnector->ExecuteJs = UOpenfortJSConnector::FOnExecuteJsDelegate::CreateUObject(this, &UOpenfortBrowserWidget::ExecuteJS);

	// WebBrowserWidget->LoadString("<html><head><title>Test</title></head><body><h1>Test</h1></body></html>",
	// TEXT("http://www.google.com")); InitialURL = TEXT("http://www.google.com");
	// InitialURL = TEXT("chrome://version");
	// IPluginManager& PluginManager = IPluginManager::Get();
	// if (const TSharedPtr<IPlugin> Plugin =
	// PluginManager.FindPlugin("Openfort"))
	// {
	//     InitialURL = FString::Printf(TEXT("%s%s"), TEXT("file:///"),
	//     *FPaths::ConvertRelativePathToFull(FPaths::Combine(Plugin->GetContentDir(),
	//     TEXT("index.html")))); OPENFORT_LOG("Loading initial url: %s",
	//     *InitialURL)
	// }
	InitialURL = TEXT("about:blank");
}

void UOpenfortBrowserWidget::BindConnector()
{
	if (JSConnector && JSConnector->IsBound())
	{
		return;
	}

	OPENFORT_LOG("Setting up %s...", *UOpenfortJSConnector::StaticClass()->GetName())

	if (JSConnector)
	{
		if (BindUObject(UOpenfortJSConnector::JSObjectName(), JSConnector))
		{
			JSConnector->Init(IsPageLoaded());
		}
	}
}

TWeakObjectPtr<UOpenfortJSConnector> UOpenfortBrowserWidget::GetJSConnector() const
{
	return JSConnector;
}

bool UOpenfortBrowserWidget::IsPageLoaded() const
{
#if USING_BUNDLED_CEF
	return WebBrowserWidget.IsValid() && WebBrowserWidget->IsLoaded();
#else
	return false; // Ensure this is reachable
#endif
}

void UOpenfortBrowserWidget::ExecuteJS(const FString &ScriptText) const
{
#if USING_BUNDLED_CEF
	if (WebBrowserWidget.IsValid())
	{
		WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
#endif
}

void UOpenfortBrowserWidget::SetBrowserContent()
{
#if USING_BUNDLED_CEF
	if (!WebBrowserWidget.IsValid())
	{
		OPENFORT_ERR("WebBrowserWidget is not valid");
		return; // This will make subsequent code unreachable if WebBrowserWidget is not valid
	}

	FSoftObjectPath AssetRef(TEXT("/Script/Openfort.OpenfortSDKResource'/Openfort/PackagedResources/index.index'"));
	if (UObject* LoadedAsset = AssetRef.TryLoad())
	{
		if (auto Resource = Cast<UOpenfortSDKResource>(LoadedAsset))
		{
			const FString IndexHtml = FString("<!doctype html><html lang='en'><head><meta charset='utf-8'><title>GameSDK Bridge</title><script>") +
				Resource->Js + FString("</script></head><body><h1>Bridge Running</h1></body></html>");

			WebBrowserWidget->LoadString(IndexHtml, TEXT("file:///openfort/index.html"));
		}
	}
#endif
}

bool UOpenfortBrowserWidget::BindUObject(const FString &Name, UObject *Object, const bool bIsPermanent) const
{
#if USING_BUNDLED_CEF
	if (!WebBrowserWidget)
	{
		OPENFORT_WARN_FUNC("Could not bind UObject '%s' to browser, WebBrowserWidget is null", *Object->GetName())
		return false;
	}

	WebBrowserWidget->BindUObject(Name, Object, bIsPermanent);
#endif
	return true;
}

void UOpenfortBrowserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
#if USING_BUNDLED_CEF
	WebBrowserWidget.Reset();
#endif
}

TSharedRef<SWidget> UOpenfortBrowserWidget::RebuildWidget()
{
	if (IsDesignTime())
	{
		return SNew(SBox).HAlign(HAlign_Center).VAlign(VAlign_Center)[SNew(STextBlock).Text(NSLOCTEXT("Openfort", "Openfort Web Browser", "Openfort Web Browser"))];
	}
	else
	{
#if USING_BUNDLED_CEF
		WebBrowserWidget = SNew(SWebBrowser).InitialURL(InitialURL).ShowControls(false).SupportsTransparency(bSupportsTransparency).ShowInitialThrobber(bShowInitialThrobber)
#if PLATFORM_ANDROID | PLATFORM_IOS
							   .OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadCompleted))
#endif
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
							   .OnConsoleMessage(BIND_UOBJECT_DELEGATE(FOnConsoleMessageDelegate, HandleOnConsoleMessage))
#endif
			;

		return WebBrowserWidget.ToSharedRef();
#else
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)[SNew(STextBlock)
									   .Text(NSLOCTEXT("Openfort",
													   "Openfort Web Browser",
													   "Openfort Web Browser"))];
#endif
	}
}

#if PLATFORM_ANDROID | PLATFORM_IOS
void UOpenfortBrowserWidget::HandleOnLoadCompleted()
{
	FString indexUrl = "file:///openfort/index.html";

#if USING_BUNDLED_CEF
	if (WebBrowserWidget->GetUrl() == indexUrl)
	{
		JSConnector->SetMobileBridgeReady();
	}
	else
	{
		OPENFORT_ERR("Openfort Browser Widget Url don't match: (loaded : %s, required: %s)", *WebBrowserWidget->GetUrl(), *indexUrl);
	}
#endif
}
#endif

void UOpenfortBrowserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	BindConnector();
	SetBrowserContent();
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
void UOpenfortBrowserWidget::HandleOnConsoleMessage(const FString &Message, const FString &Source, int32 Line, EWebBrowserConsoleLogSeverity Severity)
{
	// TODO: add severity to log and callback
	OPENFORT_LOG("Browser console message: %s, Source: %s, Line: %d", *Message, *Source, Line);
	OnConsoleMessage.Broadcast(Message, Source, Line);
}
#endif
