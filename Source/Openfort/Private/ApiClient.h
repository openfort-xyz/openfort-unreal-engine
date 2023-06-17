#pragma once
#include "HttpFwd.h"

struct FSignatureTransactionIntentComplete;
struct FSignatureSessionComplete;
struct FRequestComplete;

class ApiClient
{
private:
	FString apiKey;
	FString basePath;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PrepareRequest();

	void ProcessSignatureSessionResponse(FHttpResponsePtr Response, FSignatureSessionComplete OnComplete);

	void ProcessSignatureTransactionIntentResponse(FHttpResponsePtr Response, FSignatureTransactionIntentComplete OnComplete);

public:
	static const FString DEFAULT_BASE_PATH;

	ApiClient();
	ApiClient(FString apiKey, FString basePath = DEFAULT_BASE_PATH);

	void SignatureSession(FString sessionId, FString signature, FSignatureSessionComplete OnComplete);

	void SignatureTransactionIntent(FString sessionId, FString signature, FSignatureTransactionIntentComplete OnComplete);
};
