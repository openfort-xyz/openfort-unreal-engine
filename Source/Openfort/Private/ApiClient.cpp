#include "ApiClient.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "../Public/OpenfortComponent.h"
#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/MessageDialog.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Templates/SharedPointer.h"

const FString ApiClient::DEFAULT_BASE_PATH = TEXT("https://api.openfort.xyz");

ApiClient::ApiClient()
{
}

ApiClient::ApiClient(FString apiKey, FString basePath) : apiKey(apiKey),
	basePath(basePath)
{
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> ApiClient::PrepareRequest()
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule.CreateRequest();

	pRequest->SetVerb(TEXT("POST"));

	pRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	pRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + apiKey);
	return pRequest;
}

void LogConnectionProblem(FHttpRequestPtr pRequest)
{
	switch (pRequest->GetStatus()) {
	case EHttpRequestStatus::Failed_ConnectionError:
		UE_LOG(LogTemp, Error, TEXT("Connection failed."));
	default:
		UE_LOG(LogTemp, Error, TEXT("Request failed."));
	}
}

void ApiClient::SignatureSession(FString session, FString signature, FSignatureSessionComplete OnComplete)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = PrepareRequest();

	FString requestContent = TEXT("signature=") + signature;
	pRequest->SetContentAsString(requestContent);

	FString uri = basePath + TEXT("/v1/sessions/") + FGenericPlatformHttp::UrlEncode(session) + TEXT("/signature");
	pRequest->SetURL(uri);

	pRequest->OnProcessRequestComplete().BindLambda(
		[&](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {

		if (connectedSuccessfully) {
			UE_LOG(LogTemp, Log, TEXT("Succesfully receive respnse from session signature api."));
			ProcessSignatureSessionResponse(pResponse, OnComplete);
		}
		else {
			LogConnectionProblem(pRequest);
		}
	});

	pRequest->ProcessRequest();
}

void ApiClient::ProcessSignatureSessionResponse(const FHttpResponsePtr Response, FSignatureSessionComplete OnComplete)
{
	check(IsInGameThread());
	FString responseContent = Response->GetContentAsString();
	
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(responseContent);
	TSharedPtr<FJsonObject> JsonObject;
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot parse response."));
		return;
	}

	TSharedPtr<FSignatureSessionResponse> Result = MakeShareable(new FSignatureSessionResponse);
	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FSignatureSessionResponse::StaticStruct(), Result.Get(), 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Response has unsupported format."));
		return;
	}

	OnComplete.Broadcast(*Result.Get());
}

void ApiClient::SignatureTransactionIntent(FString transactionIntent, FString signature, FSignatureTransactionIntentComplete OnComplete)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = PrepareRequest();

	FString requestContent = TEXT("signature=") + signature;
	pRequest->SetContentAsString(requestContent);

	FString uri = basePath + TEXT("/v1/transaction_intents/") + FGenericPlatformHttp::UrlEncode(transactionIntent) + TEXT("/signature");
	pRequest->SetURL(uri);

	pRequest->OnProcessRequestComplete().BindLambda(
		[&](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {

		if (connectedSuccessfully) {
			UE_LOG(LogTemp, Log, TEXT("Succesfully receive respnse from session signature api."));
			ProcessSignatureTransactionIntentResponse(pResponse, OnComplete);
		}
		else {
			LogConnectionProblem(pRequest);
		}
	});

	pRequest->ProcessRequest();
}

void ApiClient::ProcessSignatureTransactionIntentResponse(const FHttpResponsePtr Response, FSignatureTransactionIntentComplete OnComplete)
{
	check(IsInGameThread());
	FString responseContent = Response->GetContentAsString();
	
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(responseContent);
	TSharedPtr<FJsonObject> JsonObject;
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot parse response."));
		return;
	}

	TSharedPtr<FTransactionIntentResponse> Result = MakeShareable(new FTransactionIntentResponse);
	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FTransactionIntentResponse::StaticStruct(), Result.Get(), 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Response has unsupported format."));
		return;
	}

	OnComplete.Broadcast(*Result.Get());
}
