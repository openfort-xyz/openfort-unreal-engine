// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKGetAccessTokenAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKGetAccessTokenAsyncAction *UOpenfortOpenfortSDKGetAccessTokenAsyncAction::GetAccessToken(UObject *WorldContextObject)
{
	UOpenfortOpenfortSDKGetAccessTokenAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKGetAccessTokenAsyncAction>();
	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKGetAccessTokenAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "GetAccessToken failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKGetAccessTokenAsyncAction::DoGetAccessToken);
}

void UOpenfortOpenfortSDKGetAccessTokenAsyncAction::DoGetAccessToken(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run GetAccessToken
	OpenfortSDK->GetAccessToken(UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKGetAccessTokenAsyncAction::OnGetAccessTokenResponse));
}

void UOpenfortOpenfortSDKGetAccessTokenAsyncAction::OnGetAccessTokenResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		GotAccessToken.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}
