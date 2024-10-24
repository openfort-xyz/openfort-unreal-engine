#include "Openfort/Actions/OpenfortOpenfortSDKExecuteTransactionAsyncAction.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UOpenfortOpenfortSDKExecuteTransactionAsyncAction *UOpenfortOpenfortSDKExecuteTransactionAsyncAction::ExecuteTransaction(UObject *WorldContextObject, const FString &AccessToken)
{
	UOpenfortOpenfortSDKExecuteTransactionAsyncAction *OpenfortSDKExecuteTransactionBlueprintNode = NewObject<UOpenfortOpenfortSDKExecuteTransactionAsyncAction>();
	OpenfortSDKExecuteTransactionBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKExecuteTransactionBlueprintNode->AccessToken = AccessToken;
	return OpenfortSDKExecuteTransactionBlueprintNode;
}

void UOpenfortOpenfortSDKExecuteTransactionAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "EVM Execute Transaction failed due to missing world or world context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""), TEXT(""));
		return;
	}

	DoExecuteTransaction();
}

void UOpenfortOpenfortSDKExecuteTransactionAsyncAction::DoExecuteTransaction()
{
	// Create HTTP request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TEXT("https://openfort-auth-non-custodial.vercel.app/api/protected-collect"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));

	// Set up callback for when the request completes
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UOpenfortOpenfortSDKExecuteTransactionAsyncAction::OnHttpRequestComplete);

	// Send the request
	HttpRequest->ProcessRequest();
}

void UOpenfortOpenfortSDKExecuteTransactionAsyncAction::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		FString ResponseString = Response->GetContentAsString();
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FString UserOperationHash, TransactionIntentId;
			JsonObject->TryGetStringField(TEXT("userOperationHash"), UserOperationHash);
			JsonObject->TryGetStringField(TEXT("transactionIntentId"), TransactionIntentId);

			TransactionSent.Broadcast(TEXT(""), TransactionIntentId, UserOperationHash);
		}
		else
		{
			Failed.Broadcast(TEXT("Failed to parse JSON response"), TEXT(""), TEXT(""));
		}
	}
	else
	{
		Failed.Broadcast(TEXT("HTTP Request failed"), TEXT(""), TEXT(""));
	}
}