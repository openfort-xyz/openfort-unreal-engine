// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortAuthenticateAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortConnectionAsyncActions *UOpenfortConnectionAsyncActions::AuthenticateWithOAuth(UObject *WorldContextObject)
{
	UOpenfortConnectionAsyncActions *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortConnectionAsyncActions>();

	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->bIsAuthenticate = false;

	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortConnectionAsyncActions::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Error = "Connect failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Error)
		Failed.Broadcast(Error);

		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortConnectionAsyncActions::DoAuthenticate);
}

void UOpenfortConnectionAsyncActions::DoAuthenticate(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();

	if (OpenfortSDK.IsValid())
	{
		FOAuthInitRequest Request;
		OpenfortSDK->AuthenticateWithOAuth(Request, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortConnectionAsyncActions::OnAuthenticate));
	}
	else
	{
		OPENFORT_ERR("OpenfortSDK was not valid while trying to connect")
	}
}

void UOpenfortConnectionAsyncActions::OnAuthenticate(FOpenfortOpenfortSDKResult Result)
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
