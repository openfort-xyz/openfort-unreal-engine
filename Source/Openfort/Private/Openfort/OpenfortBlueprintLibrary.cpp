#include "Openfort/OpenfortBlueprintLibrary.h"

void UOpenfortBlueprintLibrary::BreakFEvmTransactionReceiptLog(FEvmTransactionReceiptLog Log, FString &Address, FString &Data,
															   FString &BlockNumber, FString &TransactionHash, FString &TransactionIndex, FString &BlockHash, FString &LogIndex, bool &Removed,
															   TArray<FString> &Topics)
{
	Address = Log.address;
	Data = Log.data;
	BlockNumber = Log.blockNumber;
	TransactionHash = Log.transactionHash;
	TransactionIndex = Log.transactionIndex;
	BlockHash = Log.blockHash;
	LogIndex = Log.logIndex;
	Removed = Log.removed;
	Topics = Log.topics;
}

void UOpenfortBlueprintLibrary::BreakEvmTransactionReceipt(FEvmTransactionReceipt Receipt, FString &BlockHash, FString &BlockNumber,
														   FString &ContractAddress, FString &CumulativeGasUsed, FString &EffectiveGasPrice, FString &From, FString &GasUsed, FString &LogsBloom,
														   FString &Status, FString &To, FString &TransactionHash, FString &TransactionIndex, FString &Type, TArray<FEvmTransactionReceiptLog> &Logs)
{
	BlockHash = Receipt.blockHash;
	BlockNumber = Receipt.blockNumber;
	ContractAddress = Receipt.contractAddress;
	CumulativeGasUsed = Receipt.cumulativeGasUsed;
	EffectiveGasPrice = Receipt.effectiveGasPrice;
	From = Receipt.from;
	GasUsed = Receipt.gasUsed;
	LogsBloom = Receipt.logsBloom;
	Status = Receipt.status;
	To = Receipt.to;
	TransactionHash = Receipt.transactionHash;
	TransactionIndex = Receipt.transactionIndex;
	Type = Receipt.type;
	Logs = Receipt.logs;
}

void UOpenfortBlueprintLibrary::BreakFTransactionIntentResponse(FTransactionIntentResponse TinResponse, FEvmTransactionReceipt &Response, FString &Id)
{
	Response = TinResponse.response;
	Id = TinResponse.id;
}