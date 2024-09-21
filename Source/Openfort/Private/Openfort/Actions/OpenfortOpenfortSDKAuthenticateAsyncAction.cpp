//Fill out your copyright notice in the Description page of Project Settings.

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
		OPENFORT_WARN("%s", *Error);
		Failed.Broadcast(Error); // Inform listeners of the failure

		return;
	}

	auto Subsystem = GetSubsystem();
	if (Subsystem)
	{
		Subsystem->WhenReady(this, &UOpenfortOpenfortSDKAuthenticateAsyncActions::DoAuthenticate);
	}
	else
	{
		OPENFORT_ERR("Failed to get subsystem");
		// Handle the error case appropriately, e.g., broadcast an error
		FString Error = "Failed to get Openfort subsystem.";
		Failed.Broadcast(Error);
	}
}


void UOpenfortOpenfortSDKAuthenticateAsyncActions::DoAuthenticate(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Assuming GetOpenfortSDK() returns a raw pointer
	UOpenfortOpenfortSDK* OpenfortSDKPtr = GetSubsystem()->GetOpenfortSDK();

	// Convert to TWeakObjectPtr for safety
	TWeakObjectPtr<UOpenfortOpenfortSDK> OpenfortSDK(OpenfortSDKPtr);

	if (OpenfortSDK.IsValid())
	{
		FOAuthInitRequest Request(Provider, Options, bUsePooling);
		OpenfortSDK->AuthenticateWithOAuth(Request, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKAuthenticateAsyncActions::OnAuthenticate));
	}
	else
	{
		OPENFORT_ERR("OpenfortSDK was not valid while trying to connect");
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