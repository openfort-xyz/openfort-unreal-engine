// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKRequestWalletSessionKeyAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKRequestWalletSessionKey *UOpenfortOpenfortSDKRequestWalletSessionKey::RequestWalletSessionKey(UObject *WorldContextObject, const FRegisterSessionRequest &Request)
{
	UOpenfortOpenfortSDKRequestWalletSessionKey *RequestWalletSessionKeyBPNode = NewObject<UOpenfortOpenfortSDKRequestWalletSessionKey>();

	RequestWalletSessionKeyBPNode->WorldContextObject = WorldContextObject;
	RequestWalletSessionKeyBPNode->TransactionRequest = Request;
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
	// Assuming GetOpenfortSDK() returns a raw pointer
	UOpenfortOpenfortSDK* OpenfortSDKPtr = GetSubsystem()->GetOpenfortSDK();

	// Convert to TWeakObjectPtr for safety
	TWeakObjectPtr<UOpenfortOpenfortSDK> OpenfortSDK(OpenfortSDKPtr);

	if (OpenfortSDK.IsValid())
	{
		OpenfortSDK->SendSignatureSessionRequest(TransactionRequest,
			UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this,
				&UOpenfortOpenfortSDKRequestWalletSessionKey::OnRequestWalletSessionKeyResponse));
	}
	else
	{
		OPENFORT_ERR("OpenfortSDK was not valid while trying to request wallet session key.");
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
