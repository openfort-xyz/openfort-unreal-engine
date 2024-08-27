﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/OpenfortSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "OpenfortBlui.h"
#include "OpenfortBrowserUserWidget.h"
#include "Openfort/OpenfortJSConnector.h"

UOpenfortSubsystem::UOpenfortSubsystem() { OPENFORT_LOG_FUNCSIG }

void UOpenfortSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	OPENFORT_LOG_FUNCSIG
	Super::Initialize(Collection);

#if PLATFORM_ANDROID
	// Enable DOM storage so we can use localStorage in the Android webview
	GConfig->SetBool(
		TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"),
		TEXT("bEnableDomStorage"), true, GEngineIni);
#endif

#if PLATFORM_ANDROID | PLATFORM_IOS
	EngineInitCompleteHandle =
		FCoreDelegates::OnFEngineLoopInitComplete.AddUObject(
			this, &UOpenfortSubsystem::OnViewportCreated);
#else
	ViewportCreatedHandle = UGameViewportClient::OnViewportCreated().AddUObject(this, &UOpenfortSubsystem::OnViewportCreated);
#endif
	WorldTickHandle = FWorldDelegates::OnWorldTickStart.AddUObject(this, &UOpenfortSubsystem::WorldTickStart);
}

void UOpenfortSubsystem::Deinitialize()
{
	OPENFORT_LOG_FUNCSIG

	BrowserWidget = nullptr;

#if USING_BLUI_CEF
	OpenfortBlui->ConditionalBeginDestroy();
	OpenfortBlui = nullptr;
#endif

	OpenfortSDK = nullptr;

#if PLATFORM_ANDROID | PLATFORM_IOS
	UGameViewportClient::OnViewportCreated().Remove(EngineInitCompleteHandle);
#else
	UGameViewportClient::OnViewportCreated().Remove(ViewportCreatedHandle);
#endif
	FWorldDelegates::OnWorldTickStart.Remove(WorldTickHandle);

	Super::Deinitialize();
}

template <class UserClass>
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
void UOpenfortSubsystem::WhenReady(UserClass *Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TMethodPtr<UserClass> Func)
#else
void UOpenfortSubsystem::WhenReady(UserClass *Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
#endif
{
	OnReady.AddUObject(Object, Func);
}

void UOpenfortSubsystem::OnBridgeReady()
{
	// When the bridge is ready our subsystem is ready to be used by game code.
	// Set the bIsReady flag and broadcast the OnReady event for any waiting
	// delegates.
	bIsReady = true;
	ManageBridgeDelegateQueue();
}

void UOpenfortSubsystem::ManageBridgeDelegateQueue()
{
	if (bIsReady)
	{
#if USING_BLUI_CEF
		OnReady.Broadcast(OpenfortBlui->GetJSConnector());
#else
		OnReady.Broadcast(BrowserWidget->GetJSConnector());
#endif
		OnReady.Clear();
	}
}

void UOpenfortSubsystem::SetupGameBridge()
{
	if (bHasSetupGameBridge)
	{
		return;
	}
	bHasSetupGameBridge = true;

#if USING_BLUI_CEF
	// Create the Blui
	if (!OpenfortBlui)
	{
		OpenfortBlui = NewObject<UOpenfortBlui>();
		OpenfortBlui->Init();
	}

	if (!OpenfortBlui)
	{
		OPENFORT_ERR("Failed to create UOpenfortBlui")
		return;
	}
	if (!OpenfortBlui->GetJSConnector().IsValid())
	{
		OPENFORT_ERR(
			"JSConnector not available, can't set up subsystem-ready event chain")
		return;
	}
	// Set up ready event chain
	if (!IsReady())
	{
		OpenfortBlui->GetJSConnector()->AddCallbackWhenBridgeReady(
			UOpenfortJSConnector::FOnBridgeReadyDelegate::FDelegate::CreateUObject(
				this, &UOpenfortSubsystem::OnBridgeReady));
	}

	// Prepare OpenfortSDK
	if (!OpenfortSDK)
	{
		OpenfortSDK = NewObject<UOpenfortOpenfortSDK>(this);
		if (OpenfortSDK)
			OpenfortSDK->Setup(OpenfortBlui->GetJSConnector());
	}

#else
	// Create the browser widget
	if (!BrowserWidget)
	{
		BrowserWidget = CreateWidget<UOpenfortBrowserUserWidget>(GetWorld());
	}
	if (!BrowserWidget)
	{
		OPENFORT_ERR("Failed to create up BrowserWidget")
		return;
	}
	// Launch browser
	if (!BrowserWidget->IsInViewport())
	{
		OPENFORT_LOG("Adding BrowserWidget to viewport")
		BrowserWidget->AddToViewport();
	}
	if (!BrowserWidget->GetJSConnector().IsValid())
	{
		OPENFORT_ERR("JSConnector not available, can't set up subsystem-ready event chain")
		return;
	}
	// Set up ready event chain
	if (!IsReady())
	{
		BrowserWidget->GetJSConnector()->AddCallbackWhenBridgeReady(UOpenfortJSConnector::FOnBridgeReadyDelegate::FDelegate::CreateUObject(this, &UOpenfortSubsystem::OnBridgeReady));
	}

	// Prepare OpenfortSDK
	if (!OpenfortSDK)
	{
		OpenfortSDK = NewObject<UOpenfortOpenfortSDK>(this);
		if (OpenfortSDK)
		{
			OpenfortSDK->Setup(BrowserWidget->GetJSConnector());
		}
	}
#endif
}

void UOpenfortSubsystem::OnViewportCreated()
{
	OPENFORT_LOG_FUNCSIG
	SetupGameBridge();
}

void UOpenfortSubsystem::WorldTickStart(UWorld *World, ELevelTick TickType, float DeltaSeconds)
{
	ManageBridgeDelegateQueue();
}