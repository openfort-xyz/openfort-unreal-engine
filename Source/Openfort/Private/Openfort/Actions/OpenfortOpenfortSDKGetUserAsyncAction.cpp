// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKGetUserAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKGetUserAsyncAction *UOpenfortOpenfortSDKGetUserAsyncAction::GetUser(UObject *WorldContextObject)
{
	UOpenfortOpenfortSDKGetUserAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKGetUserAsyncAction>();
	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKGetUserAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "GetUser failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKGetUserAsyncAction::DoGetUser);
}

void UOpenfortOpenfortSDKGetUserAsyncAction::DoGetUser(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run GetUser
	OpenfortSDK->GetUser(UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKGetUserAsyncAction::OnGetUserResponse));
}

void UOpenfortOpenfortSDKGetUserAsyncAction::OnGetUserResponse(FOpenfortOpenfortSDKResult Result)
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
