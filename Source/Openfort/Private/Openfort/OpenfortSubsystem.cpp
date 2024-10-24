// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/OpenfortSubsystem.h"
#include <Openfort/Actions/OpenfortOpenfortSDKGetUserAsyncAction.h>
#include <Openfort/Actions/OpenfortOpenfortSDKInitializationAsyncAction.h>
#include <Openfort/Actions/OpenfortOpenfortSDKLogoutAsyncAction.h>
#include <Openfort/Actions/OpenfortOpenfortSDKRequestWalletSessionKeyAsyncAction.h>
#include <Openfort/Actions/OpenfortOpenfortSDKSignTransactionAsyncAction.h>
#include "OpenfortBrowserUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Openfort/OpenfortJSConnector.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/Actions/OpenfortOpenfortSDKAuthenticateAsyncAction.h"
#include "Openfort/Actions/OpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction.h"
#include "Openfort/Actions/OpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction.h"
#include "Openfort/Actions/OpenfortOpenfortSDKExecuteTransactionAsyncAction.h"
#include "Openfort/Actions/OpenfortOpenfortSDKGetAccessTokenAsyncAction.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortSubsystem::UOpenfortSubsystem() { OPENFORT_LOG_FUNCSIG }

void UOpenfortSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	OPENFORT_LOG_FUNCSIG
	Super::Initialize(Collection);

#if PLATFORM_ANDROID
	// Enable DOM storage for localStorage in the Android webview
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
	OpenfortSDK = nullptr;

#if PLATFORM_ANDROID | PLATFORM_IOS
	UGameViewportClient::OnViewportCreated().Remove(EngineInitCompleteHandle);
#else
	UGameViewportClient::OnViewportCreated().Remove(ViewportCreatedHandle);
#endif
	FWorldDelegates::OnWorldTickStart.Remove(WorldTickHandle);

	Super::Deinitialize();
}

TWeakObjectPtr<UOpenfortOpenfortSDK> UOpenfortSubsystem::GetOpenfortSDK() const
{
	return MakeWeakObjectPtr<UOpenfortOpenfortSDK>(OpenfortSDK);
}

// Template function should typically be in the header file, but if you keep it here:
template <class UserClass>
void UOpenfortSubsystem::WhenReady(UserClass *Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TMethodPtr<UserClass> Func)
{
	OnReady.AddUObject(Object, Func);
}

// Explicit instantiations
template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKGetAccessTokenAsyncAction>(
	UOpenfortOpenfortSDKGetAccessTokenAsyncAction *,
	void (UOpenfortOpenfortSDKGetAccessTokenAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKGetUserAsyncAction>(
	UOpenfortOpenfortSDKGetUserAsyncAction *,
	void (UOpenfortOpenfortSDKGetUserAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction>(
	UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction *,
	void (UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKInitializationAsyncAction>(
	UOpenfortOpenfortSDKInitializationAsyncAction *,
	void (UOpenfortOpenfortSDKInitializationAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKLogoutAsyncAction>(
	UOpenfortOpenfortSDKLogoutAsyncAction *,
	void (UOpenfortOpenfortSDKLogoutAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKRequestWalletSessionKey>(
	UOpenfortOpenfortSDKRequestWalletSessionKey *,
	void (UOpenfortOpenfortSDKRequestWalletSessionKey::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKSignTransactionAsyncAction>(
	UOpenfortOpenfortSDKSignTransactionAsyncAction *,
	void (UOpenfortOpenfortSDKSignTransactionAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKAuthenticateAsyncActions>(
	UOpenfortOpenfortSDKAuthenticateAsyncActions *,
	void (UOpenfortOpenfortSDKAuthenticateAsyncActions::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction>(
	UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction *,
	void (UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::*)(TWeakObjectPtr<UOpenfortJSConnector, FWeakObjectPtr>));

void UOpenfortSubsystem::OnBridgeReady()
{
	// When the bridge is ready, our subsystem is ready for use
	bIsReady = true;
	ManageBridgeDelegateQueue();
}

void UOpenfortSubsystem::ManageBridgeDelegateQueue()
{
	if (bIsReady)
	{
		OnReady.Broadcast(BrowserWidget->GetJSConnector());
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
	
	// Create the browser widget
	if (!BrowserWidget)
	{
		BrowserWidget = CreateWidget<UOpenfortBrowserUserWidget>(GetWorld());
	}
	if (!BrowserWidget)
	{
		OPENFORT_ERR("Failed to create BrowserWidget")
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
