// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKInitializationAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"

UOpenfortOpenfortSDKInitializationAsyncAction *UOpenfortOpenfortSDKInitializationAsyncAction::InitializeOpenfortSDK(UObject *WorldContextObject, const FString &ClientID, const FString &RedirectUri, const FString &LogoutUri)
{
	UOpenfortOpenfortSDKInitializationAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKInitializationAsyncAction>();

	OpenfortSDKInitBlueprintNode->ClientId = ClientID;
	OpenfortSDKInitBlueprintNode->RedirectUri = RedirectUri;
	OpenfortSDKInitBlueprintNode->LogoutUri = LogoutUri;
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
	OpenfortSDK->Initialize(FOpenfortOpenfortSDKInitData{ClientId, RedirectUri, LogoutUri}, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKInitializationAsyncAction::OnInitialized));
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
