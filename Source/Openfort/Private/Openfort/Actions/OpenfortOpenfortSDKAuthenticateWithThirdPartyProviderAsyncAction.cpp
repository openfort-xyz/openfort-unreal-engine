// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction *UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::AuthenticateWithThirdPartyProvider(UObject *WorldContextObject, const FThirdPartyProviderRequest &ThirdPartyProviderRequest)
{
	UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction>();
	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->ThirdPartyProviderRequest = ThirdPartyProviderRequest;
	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "AuthenticateWithThirdPartyProvider failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::DoAuthenticateWithThirdPartyProvider);
}

void UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::DoAuthenticateWithThirdPartyProvider(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run AuthenticateWithThirdPartyProvider
	if (OpenfortSDK.IsValid())
	{
		FThirdPartyProviderRequest Request(ThirdPartyProviderRequest);
		OpenfortSDK->AuthenticateWithThirdPartyProvider(Request, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::OnAuthenticateWithThirdPartyProviderResponse));
	}
	else
	{
		OPENFORT_ERR("OpenfortSDK was not valid while trying to connect")
	}
}

void UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction::OnAuthenticateWithThirdPartyProviderResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		GotUser.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}
