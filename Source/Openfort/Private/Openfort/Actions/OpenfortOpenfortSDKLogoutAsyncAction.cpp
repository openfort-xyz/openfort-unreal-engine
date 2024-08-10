// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKLogoutAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKLogoutAsyncAction *UOpenfortOpenfortSDKLogoutAsyncAction::Logout(UObject *WorldContextObject, bool DoHardLogout)
{
	UOpenfortOpenfortSDKLogoutAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKLogoutAsyncAction>();

	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->bDoHardLogout = DoHardLogout;

	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKLogoutAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		const FString ErrorMessage = "Logout failed due to missing world or world context object.";

		OPENFORT_WARN("%s", *ErrorMessage)
		OnFailure.Broadcast(ErrorMessage);

		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKLogoutAsyncAction::DoLogout);
}

void UOpenfortOpenfortSDKLogoutAsyncAction::DoLogout(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();

	OpenfortSDK->Logout(bDoHardLogout, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKLogoutAsyncAction::OnLogoutResponse));
}

void UOpenfortOpenfortSDKLogoutAsyncAction::OnLogoutResponse(FOpenfortOpenfortSDKResult Result) const
{
	if (Result.Success)
	{
		OnSuccess.Broadcast(Result.Message);
	}
	else
	{
		OnFailure.Broadcast(Result.Message);
	}
}
