// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKAuthenticateAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKAuthenticateAsyncActions *UOpenfortOpenfortSDKAuthenticateAsyncActions::AuthenticateWithOAuth(UObject *WorldContextObject, EOAuthProvider Provider, const FOAuthInitRequestOptions &Options, bool bUsePooling)
{
	UOpenfortOpenfortSDKAuthenticateAsyncActions *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKAuthenticateAsyncActions>();

	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->Provider = Provider;
	OpenfortSDKInitBlueprintNode->Options = Options;
	OpenfortSDKInitBlueprintNode->bUsePooling = bUsePooling;
	OpenfortSDKInitBlueprintNode->bIsAuthenticate = false;

	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKAuthenticateAsyncActions::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Error = "Connect failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Error)
		Failed.Broadcast(Error);

		return;
	}
	OPENFORT_LOG("YO: WhenReady.")
	auto Subsystem = GetSubsystem();
	if (Subsystem)
	{
		Subsystem->WhenReady(this, &UOpenfortOpenfortSDKAuthenticateAsyncActions::DoAuthenticate);
	}
	else
	{
		OPENFORT_ERR("Failed to get subsystem");
		// Handle the error case appropriately
	}
}

void UOpenfortOpenfortSDKAuthenticateAsyncActions::DoAuthenticate(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	OPENFORT_LOG("YO: GetSubsystem");
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();

	if (OpenfortSDK.IsValid())
	{
		OPENFORT_LOG("YO: action yo.");
		FOAuthInitRequest Request(Provider, Options, bUsePooling);
		OpenfortSDK->InitOAuth(Request, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKAuthenticateAsyncActions::OnAuthenticate));
		// OpenfortSDK->AuthenticateWithOAuth(Request, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKAuthenticateAsyncActions::OnAuthenticate));
	}
	else
	{
		OPENFORT_ERR("OpenfortSDK was not valid while trying to connect")
	}
}

void UOpenfortOpenfortSDKAuthenticateAsyncActions::OnAuthenticate(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		Success.Broadcast(TEXT(""));
	}
	else
	{
		Failed.Broadcast(Result.Message);
	}
}