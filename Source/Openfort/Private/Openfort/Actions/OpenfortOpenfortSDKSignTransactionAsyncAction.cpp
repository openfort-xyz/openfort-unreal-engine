#include "Openfort/Actions/OpenfortOpenfortSDKSignTransactionAsyncAction.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKSignTransactionAsyncAction *UOpenfortOpenfortSDKSignTransactionAsyncAction::SignTransaction(UObject *WorldContextObject, const FSignatureTransactionIntentRequest &Request)
{
	UOpenfortOpenfortSDKSignTransactionAsyncAction *OpenfortSDKSignTransactionBlueprintNode = NewObject<UOpenfortOpenfortSDKSignTransactionAsyncAction>();
	OpenfortSDKSignTransactionBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKSignTransactionBlueprintNode->TransactionRequest = Request;
	return OpenfortSDKSignTransactionBlueprintNode;
}

void UOpenfortOpenfortSDKSignTransactionAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "EVM Sign Transaction failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKSignTransactionAsyncAction::DoSignTransaction);
}

void UOpenfortOpenfortSDKSignTransactionAsyncAction::DoSignTransaction(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	OpenfortSDK->SendSignatureTransactionIntentRequest(TransactionRequest, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKSignTransactionAsyncAction::OnSignTransactionResponse));
}

void UOpenfortOpenfortSDKSignTransactionAsyncAction::OnSignTransactionResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		OPENFORT_LOG("EVM Sign Transaction success")
		TransactionSigned.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		OPENFORT_LOG("EVM Sign Transaction failed")
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}