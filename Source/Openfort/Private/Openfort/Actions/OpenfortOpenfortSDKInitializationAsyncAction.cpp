// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKInitializationAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"

UOpenfortOpenfortSDKInitializationAsyncAction *UOpenfortOpenfortSDKInitializationAsyncAction::InitializeOpenfortSDK(UObject *WorldContextObject, const FString &PublishableKey, const FString &ShieldPublishableKey, const FString ShieldEncryptionKey, const bool &BShieldDebug, const FString &BackendUrl, const FString &IframeUrl, const FString &ShieldUrl)
{
	UOpenfortOpenfortSDKInitializationAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKInitializationAsyncAction>();

	OpenfortSDKInitBlueprintNode->PublishableKey = PublishableKey;
	OpenfortSDKInitBlueprintNode->ShieldPublishableKey = ShieldPublishableKey;
	OpenfortSDKInitBlueprintNode->ShieldEncryptionKey = ShieldEncryptionKey;
	OpenfortSDKInitBlueprintNode->ShieldUrl = ShieldUrl;
	OpenfortSDKInitBlueprintNode->BShieldDebug = BShieldDebug;
	OpenfortSDKInitBlueprintNode->BackendUrl = BackendUrl;
	OpenfortSDKInitBlueprintNode->IframeUrl = IframeUrl;
	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;

	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKInitializationAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		Failed.Broadcast("Initialization failed due to missing world or world context object.");
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKInitializationAsyncAction::DoInit);
}

void UOpenfortOpenfortSDKInitializationAsyncAction::DoInit(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run Initialize
	OpenfortSDK->Initialize(FOpenfortOpenfortSDKInitData{PublishableKey, ShieldPublishableKey, ShieldEncryptionKey, ShieldUrl, BShieldDebug, BackendUrl, IframeUrl}, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKInitializationAsyncAction::OnInitialized));
}

void UOpenfortOpenfortSDKInitializationAsyncAction::OnInitialized(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		Initialized.Broadcast(Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message);
	}
}
