#pragma once

#include "CoreMinimal.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "JsonObjectConverter.h"
// clang-format off
#include "OpenfortJSMessages.generated.h"
// clang-format on

UENUM()
enum class EOpenfortOpenfortSDKError : int8
{
	AuthenticationError,
	WalletConnectionError,
	UserRegistrationError,
	RefreshTokenError,
	OperationNotSupportedError
	// unknown, timeout?
};

USTRUCT()
struct OPENFORT_API FOpenfortResponseError
{
	GENERATED_BODY()

	UPROPERTY()
	EOpenfortOpenfortSDKError errorType = static_cast<EOpenfortOpenfortSDKError>(-1);

	UPROPERTY()
	FString errorMessage;

	FString ToString()
	{
		const FString ErrType = StaticEnum<EOpenfortOpenfortSDKError>()->GetNameStringByValue(static_cast<int8>(errorType));
		return ErrType == "" ? errorMessage : ErrType + ": " + errorMessage;
	}
};

USTRUCT()
struct OPENFORT_API FOpenfortJSRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString fxName;

	UPROPERTY()
	FString data;

	UPROPERTY()
	FString requestId;

	FString ToJsonString() const
	{
		FString OutString;
		FJsonObjectConverter::UStructToJsonObjectString<FOpenfortJSRequest>(*this, OutString, 0, 0, 0, nullptr, false);
		return OutString;
	}
};

USTRUCT()
struct OPENFORT_API FOpenfortJSResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString responseFor;

	UPROPERTY()
	FString requestId;

	UPROPERTY()
	bool success = false;

	TOptional<FOpenfortResponseError> Error;

	// Store the parsed response object for later in case we need to access more
	// data from it (tokens, addresses, user email, etc)
	TSharedPtr<FJsonObject> JsonObject;

	static TOptional<FOpenfortJSResponse> FromJsonString(const FString &JsonString)
	{
		TOptional<FOpenfortJSResponse> Response;
		FOpenfortJSResponse JSResponse;

		// Parse the JSON
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		if (!FJsonSerializer::Deserialize(JsonReader, JSResponse.JsonObject) || !JSResponse.JsonObject.IsValid())
		{
			OPENFORT_ERR("Could not parse response from JavaScript -- invalid JSON: %s", *JsonString)
		}
		else if (!FJsonObjectConverter::JsonObjectToUStruct(JSResponse.JsonObject.ToSharedRef(), &JSResponse, 0, 0))
		{
			OPENFORT_ERR("Could not parse response from JavaScript into the expected "
						 "response object format: %s",
						 *JsonString)
		}
		else
		{
			Response = JSResponse;
		}

		if (!JSResponse.success)
		{
			double ErrType = -1;
			FString Error;
			if (JSResponse.JsonObject.IsValid())
			{
				JSResponse.JsonObject->TryGetNumberField(TEXT("errorType"), ErrType);
				JSResponse.JsonObject->TryGetStringField(TEXT("error"), Error);
			}
			JSResponse.Error = FOpenfortResponseError{static_cast<EOpenfortOpenfortSDKError>(ErrType), Error};
		}

		return Response;
	}
};
