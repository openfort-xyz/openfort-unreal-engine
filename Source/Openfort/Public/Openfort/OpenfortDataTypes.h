#pragma once

#include "JsonObjectConverter.h"
#include "Openfort/OpenfortJSMessages.h"
#include "Openfort/OpenfortNames.h"

#include "OpenfortDataTypes.generated.h"

// This is the version of the Unreal Openfort SDK that is being used. This is not the version of the engine.
// This hardcoded value will be updated by a workflow during the release process.
#define ENGINE_SDK_VERSION TEXT("0.0.1")

USTRUCT()
struct FOpenfortEngineVersionData
{
	GENERATED_BODY()

	UPROPERTY()
	FString engine = TEXT("unreal");

	UPROPERTY()
	FString engineSdkVersion = ENGINE_SDK_VERSION;

	// cannot have spaces
	UPROPERTY()
	FString engineVersion = FEngineVersion::Current().ToString().Replace(TEXT(" "), TEXT("_"));

	// cannot have spaces
	UPROPERTY()
	FString platform = FString(FPlatformProperties::IniPlatformName()).Replace(TEXT(" "), TEXT("_"));

	// cannot have spaces
	UPROPERTY()
	FString platformVersion = FPlatformMisc::GetOSVersion().Replace(TEXT(" "), TEXT("_"));

	// Information on device. Examples of expected results: Apple|Apple M3 Max, GenuineIntel|13th Gen Intel(R) Core(TM) i7-13700H, Apple|iPhone15&#44;4.
	UPROPERTY()
	FString deviceModel = FGenericPlatformMisc::GetDeviceMakeAndModel();
};

USTRUCT()
struct FOpenfortOpenfortSDKStoreCredentialsData
{
	GENERATED_BODY()

	UPROPERTY()
	FString access_token;

	UPROPERTY()
	FString refresh_token;
};

USTRUCT()
struct FOpenfortOpenfortSDKInitOAuthData
{
	GENERATED_BODY()

	UPROPERTY()
	FString url;

	UPROPERTY()
	FString key;
};

USTRUCT()
struct OPENFORT_API FOpenfortOpenfortSDKInitData
{
	GENERATED_BODY()

	UPROPERTY()
	FString publishableKey;

	UPROPERTY()
	FString shieldPublishableKey;

	UPROPERTY()
	FString shieldEncryptionKey;

	UPROPERTY()
	FString shieldUrl;

	UPROPERTY()
	bool bShieldDebug;

	UPROPERTY()
	FString backendUrl;

	UPROPERTY()
	FString iframeUrl;

	FString ToJsonString() const;
};

USTRUCT()
struct OPENFORT_API FOpenfortOpenfortSDKResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool Success = false;

	UPROPERTY()
	FString Message;

	FOpenfortJSResponse Response;
};

USTRUCT(BlueprintType, meta = (HasNativeBreak = "/Script/Openfort.OpenfortBlueprintLibrary.BreakFEvmTransactionReceiptLog"))
struct OPENFORT_API FEvmTransactionReceiptLog
{
	GENERATED_BODY()

	UPROPERTY()
	FString address;

	UPROPERTY()
	TArray<FString> topics;

	UPROPERTY()
	FString data;

	UPROPERTY()
	FString blockNumber;

	UPROPERTY()
	FString transactionHash;

	UPROPERTY()
	FString transactionIndex;

	UPROPERTY()
	FString blockHash;

	UPROPERTY()
	FString logIndex;

	UPROPERTY()
	bool removed = false;
};

USTRUCT(BlueprintType, meta = (HasNativeBreak = "/Script/Openfort.OpenfortBlueprintLibrary.BreakEvmTransactionReceipt"))
struct OPENFORT_API FEvmTransactionReceipt
{
	GENERATED_BODY()

	UPROPERTY()
	FString blockHash;

	UPROPERTY()
	FString blockNumber;

	UPROPERTY()
	FString contractAddress;

	UPROPERTY()
	FString cumulativeGasUsed;

	UPROPERTY()
	FString effectiveGasPrice;

	UPROPERTY()
	FString from;

	UPROPERTY()
	FString gasUsed;

	UPROPERTY(BlueprintReadOnly)
	TArray<FEvmTransactionReceiptLog> logs;

	UPROPERTY()
	FString logsBloom;

	// Either 1 (success) or 0 (failure) encoded as a hexadecimal.
	UPROPERTY()
	FString status;

	UPROPERTY()
	FString to;

	UPROPERTY()
	FString transactionHash;

	UPROPERTY()
	FString transactionIndex;

	UPROPERTY()
	FString type;
};
