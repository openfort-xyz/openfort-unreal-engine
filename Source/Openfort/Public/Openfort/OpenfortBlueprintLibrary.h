#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Openfort/OpenfortDataTypes.h"

#include "OpenfortBlueprintLibrary.generated.h"

UCLASS()
class OPENFORT_API UOpenfortBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Openfort", meta = (NativeBreakFunc))
	static void BreakFEvmTransactionReceiptLog(FEvmTransactionReceiptLog Log, FString &Address, FString &Data, FString &BlockNumber, FString &TransactionHash, FString &TransactionIndex, FString &BlockHash, FString &LogIndex, bool &Removed, TArray<FString> &Topics);

	UFUNCTION(BlueprintPure, Category = "Openfort", meta = (NativeBreakFunc))
	static void BreakEvmTransactionReceipt(FEvmTransactionReceipt Receipt, FString &BlockHash, FString &BlockNumber, FString &ContractAddress, FString &CumulativeGasUsed, FString &EffectiveGasPrice, FString &From, FString &GasUsed, FString &LogsBloom, FString &Status, FString &To, FString &TransactionHash, FString &TransactionIndex, FString &Type, TArray<FEvmTransactionReceiptLog> &Logs);

	UFUNCTION(BlueprintPure, Category = "Openfort", meta = (NativeBreakFunc))
	static void BreakFTransactionIntentResponse(FTransactionIntentResponse TinResponse, FEvmTransactionReceipt &Response, FString &Id);
};