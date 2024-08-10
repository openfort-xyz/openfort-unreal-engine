// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKGetIdTokenAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKGetIdTokenAsyncAction *UOpenfortOpenfortSDKGetIdTokenAsyncAction::GetIdToken(UObject *WorldContextObject)
{
	UOpenfortOpenfortSDKGetIdTokenAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKGetIdTokenAsyncAction>();
	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKGetIdTokenAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "GetIdToken failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKGetIdTokenAsyncAction::DoGetIdToken);
}

void UOpenfortOpenfortSDKGetIdTokenAsyncAction::DoGetIdToken(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run GetIdToken
	OpenfortSDK->GetIdToken(UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKGetIdTokenAsyncAction::OnGetIdTokenResponse));
}

void UOpenfortOpenfortSDKGetIdTokenAsyncAction::OnGetIdTokenResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		GotIdToken.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}
