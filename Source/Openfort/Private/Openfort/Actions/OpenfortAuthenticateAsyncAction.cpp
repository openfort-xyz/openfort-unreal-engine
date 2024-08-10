// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortAuthenticateAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortConnectionAsyncActions *UOpenfortConnectionAsyncActions::Authenticate(UObject *WorldContextObject, bool UseCachedSession)
{
	UOpenfortConnectionAsyncActions *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortConnectionAsyncActions>();

	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->bUseCachedSession = UseCachedSession;
	OpenfortSDKInitBlueprintNode->bIsAuthenticate = false;

	return OpenfortSDKInitBlueprintNode;
}

UOpenfortConnectionAsyncActions *UOpenfortConnectionAsyncActions::AuthenticatePKCE(UObject *WorldContextObject)
{
	UOpenfortConnectionAsyncActions *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortConnectionAsyncActions>();

	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->bIsAuthenticate = false;
	OpenfortSDKInitBlueprintNode->bIsPKCE = true;

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

	GetSubsystem()->WhenReady(this, &UOpenfortConnectionAsyncActions::DoConnect);
}

void UOpenfortConnectionAsyncActions::DoConnect(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();

	if (OpenfortSDK.IsValid())
	{
		if (bIsPKCE)
		{
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
			OpenfortSDK->AuthenticatePKCE(UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortConnectionAsyncActions::OnConnect));
#endif
		}
		else
		{
			OpenfortSDK->Authenticate(bUseCachedSession, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortConnectionAsyncActions::OnConnect));
		}
	}
	else
	{
		OPENFORT_ERR("OpenfortSDK was not valid while trying to connect")
	}
}

void UOpenfortConnectionAsyncActions::OnConnect(FOpenfortOpenfortSDKResult Result)
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
