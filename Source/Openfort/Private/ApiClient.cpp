#include "ApiClient.h"

#include "HttpModule.h"
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

void ApiClient::SignatureSession(FString session, FString signature)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule.CreateRequest();

    pRequest->SetVerb(TEXT("POST"));

	pRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	pRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + apiKey);

    FString RequestContent = TEXT("signature=") + signature;
    pRequest->SetContentAsString(RequestContent);

    FString uri = basePath + TEXT("/v1/sessions/") + FGenericPlatformHttp::UrlEncode(session) + TEXT("/signature");
    pRequest->SetURL(uri);

    pRequest->OnProcessRequestComplete().BindLambda(
        [&](
            FHttpRequestPtr pRequest,
            FHttpResponsePtr pResponse,
            bool connectedSuccessfully) mutable {

        if (connectedSuccessfully) {
        	UE_LOG(LogTemp, Log, TEXT("Succesfully receive respnse from session signature api."));
            ProcessSignatureSessionResponse(pResponse);
        }
        else {
            switch (pRequest->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
                UE_LOG(LogTemp, Error, TEXT("Connection failed."));
            default:
                UE_LOG(LogTemp, Error, TEXT("Request failed."));
            }
        }
    });

    // Finally, submit the request for processing
    pRequest->ProcessRequest();
}

void ApiClient::ProcessSignatureSessionResponse(const FHttpResponsePtr Response)
{
	check(IsInGameThread());
	FString responseContent = Response->GetContentAsString();

	//UE_LOG(LogTemp, Log, responseContent.GetCharArray());
	//FText dialog = FText::FromString(responseContent);
	//FMessageDialog::Open(EAppMsgType::Ok, dialog);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(responseContent);
	/*TArray<TSharedPtr<FJsonValue>> OutArray;

	if (FJsonSerializer::Deserialize(JsonReader, OutArray))
	{
		ProcessSignatureSessionResponse(OutArray);
	}*/
}