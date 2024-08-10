#include "Openfort/Actions/OpenfortOpenfortSDKHasStoredCredentialsAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction *UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction::HasStoredCredentials(UObject *WorldContextObject)
{
	UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction>();

	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;

	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "HasStoredCredentials failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		OnFalse.Broadcast(Err);
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction::DoHasStoredCredentials);
}

void UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction::DoHasStoredCredentials(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	const auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();

	if (OpenfortSDK.IsValid())
	{
		OpenfortSDK->HasStoredCredentials(UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction::OnHasStoredCredentialsResponse));
	}
}

void UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction::OnHasStoredCredentialsResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		OnTrue.Broadcast(TEXT(""));
	}
	else
	{
		OnFalse.Broadcast(Result.Message);
	}
}
