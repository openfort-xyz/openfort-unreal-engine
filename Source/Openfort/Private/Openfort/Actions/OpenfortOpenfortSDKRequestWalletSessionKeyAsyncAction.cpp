// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKRequestWalletSessionKeyAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKRequestWalletSessionKey *UOpenfortOpenfortSDKRequestWalletSessionKey::RequestWalletSessionKey(
	UObject *WorldContextObject)
{
	UOpenfortOpenfortSDKRequestWalletSessionKey *RequestWalletSessionKeyBPNode = NewObject<UOpenfortOpenfortSDKRequestWalletSessionKey>();

	RequestWalletSessionKeyBPNode->WorldContextObject = WorldContextObject;

	return RequestWalletSessionKeyBPNode;
}

void UOpenfortOpenfortSDKRequestWalletSessionKey::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "Request Wallet Session Key failed due to missing world context object.";

		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err);

		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKRequestWalletSessionKey::DoRequestWalletSessionKey);
}

void UOpenfortOpenfortSDKRequestWalletSessionKey::DoRequestWalletSessionKey(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();

	if (OpenfortSDK.IsValid())
	{
		OpenfortSDK->RequestWalletSessionKey(UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKRequestWalletSessionKey::OnRequestWalletSessionKeyResponse));
	}
}

void UOpenfortOpenfortSDKRequestWalletSessionKey::OnRequestWalletSessionKeyResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		OPENFORT_LOG("Request Wallet Session Key successful")
		Success.Broadcast(TEXT(""));
	}
	else
	{
		OPENFORT_LOG("Request Wallet Session Key failed")
		Failed.Broadcast(Result.Message);
	}
}
