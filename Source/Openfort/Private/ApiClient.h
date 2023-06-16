#pragma once
#include "HttpFwd.h"

class ApiClient
{
private:
	FString apiKey;
	FString basePath;

	void ProcessSignatureSessionResponse(FHttpResponsePtr Response);

public:
	static const FString DEFAULT_BASE_PATH;

	ApiClient();
	ApiClient(FString apiKey, FString basePath = DEFAULT_BASE_PATH);

	void SignatureSession(FString sessionId, FString signature);
};
