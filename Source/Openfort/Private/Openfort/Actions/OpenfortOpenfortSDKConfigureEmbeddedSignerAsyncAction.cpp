// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction *UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::ConfigureEmbeddedSigner(UObject *WorldContextObject, const FEmbeddedSignerRequest &ShieldConfig)
{
	UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction *OpenfortSDKInitBlueprintNode = NewObject<UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction>();
	OpenfortSDKInitBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKInitBlueprintNode->ShieldConfig = ShieldConfig;
	return OpenfortSDKInitBlueprintNode;
}

void UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "ConfigureEmbeddedSigner failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::DoConfigureEmbeddedSigner);
}

void UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::DoConfigureEmbeddedSigner(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run ConfigureEmbeddedSigner
	FEmbeddedSignerRequest Request(ShieldConfig);
	OpenfortSDK->ConfigureEmbeddedSigner(Request, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::OnConfigureEmbeddedSignerResponse));
}

void UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction::OnConfigureEmbeddedSignerResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		Success.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}
