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
struct OPENFORT_API FOpenfortOpenfortSDKInitData
{
	GENERATED_BODY()

	UPROPERTY()
	FString clientId;

	UPROPERTY()
	FString redirectUri;

	UPROPERTY()
	FString logoutRedirectUri;

	FString ToJsonString() const;
};

USTRUCT()
struct FOpenfortOpenfortSDKInitDeviceFlowData
{
	GENERATED_BODY()

	UPROPERTY()
	FString code;

	UPROPERTY()
	FString deviceCode;

	UPROPERTY()
	FString url;

	UPROPERTY()
	float interval = 0;

	static TOptional<FOpenfortOpenfortSDKInitDeviceFlowData> FromJsonString(const FString &JsonObjectString);
};

USTRUCT()
struct FOpenfortOpenfortSDKCodeConfirmRequestData
{
	GENERATED_BODY()

	UPROPERTY()
	FString deviceCode;

	UPROPERTY()
	float interval = 5;

	UPROPERTY()
	float timeoutMs = 15 * 60 * 1000;
};

USTRUCT()
struct FOpenfortOpenfortSDKConnectPKCEData
{
	GENERATED_BODY()

	UPROPERTY()
	FString authorizationCode;

	UPROPERTY()
	FString state;
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

USTRUCT(BlueprintType)
struct FOpenfortAccessListItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString address;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> storageKeys;
};

USTRUCT(BlueprintType)
struct FNftTransferDetails
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString receiver;

	UPROPERTY(BlueprintReadWrite)
	FString tokenId;

	UPROPERTY(BlueprintReadWrite)
	FString tokenAddress;
};

USTRUCT(BlueprintType, meta = (HasNativeBreak = "/Script/Openfort.OpenfortBlueprintLibrary.BreakFZkEvmTransactionReceiptLog"))
struct OPENFORT_API FZkEvmTransactionReceiptLog
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

USTRUCT(BlueprintType, meta = (HasNativeBreak = "/Script/Openfort.OpenfortBlueprintLibrary.BreakZkEvmTransactionReceipt"))
struct OPENFORT_API FZkEvmTransactionReceipt
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
	TArray<FZkEvmTransactionReceiptLog> logs;

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
