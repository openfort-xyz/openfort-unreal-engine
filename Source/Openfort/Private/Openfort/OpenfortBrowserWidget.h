﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if USING_BUNDLED_CEF
#include "IWebBrowserWindow.h"
#endif
#include "Components/Widget.h"
#include "OpenfortBrowserWidget.generated.h"

UCLASS()
class OPENFORT_API UOpenfortBrowserWidget : public UWidget
{
	GENERATED_BODY()

	friend class UOpenfortJSConnector;

public:
	// Sets default values for this actor's properties
	UOpenfortBrowserWidget();

	// Get a pointer to the JSConnector
	TWeakObjectPtr<class UOpenfortJSConnector> GetJSConnector() const;

	bool IsPageLoaded() const;

protected:
	void ExecuteJS(const FString &ScriptText) const;

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnConsoleMessage, const FString &, Message, const FString &, Source, int32, Line);

	FOnConsoleMessage OnConsoleMessage;

#if USING_BUNDLED_CEF
	TSharedPtr<class SWebBrowser> WebBrowserWidget;
#endif
	/** URL that the browser will initially navigate to. The URL should include
	 * the protocol, eg http:// */
	FString InitialURL;
	/** Should the browser window support transparency. */
	bool bSupportsTransparency = true;
	bool bShowInitialThrobber = false;

	UPROPERTY()
	class UOpenfortJSConnector *JSConnector = nullptr;

	void SetBrowserContent();

	bool BindUObject(const FString &Name, UObject *Object, bool bIsPermanent = true) const;
	// Bind the JSConnector to the browser window
	void BindConnector();

#if PLATFORM_ANDROID | PLATFORM_IOS
	void HandleOnLoadCompleted();
#endif

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	void HandleOnConsoleMessage(const FString &Message, const FString &Source, int32 Line, EWebBrowserConsoleLogSeverity Severity);
#endif

	/**
	 * UWidget interface
	 */
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
};
