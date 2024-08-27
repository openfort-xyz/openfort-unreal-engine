// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKExecuteTransactionAsyncAction.generated.h"

/**
 * Async action blueprint node for EVM Execute Transaction
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKExecuteTransactionAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKExecuteTransactionOutputPin, FString, ErrorMessage, FString, Transaction);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKExecuteTransactionAsyncAction *ExecuteTransaction(UObject *WorldContextObject, const FSignatureTransactionIntentRequest &Request);

	virtual void Activate() override;

private:
	FSignatureTransactionIntentRequest TransactionRequest;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKExecuteTransactionOutputPin TransactionSent;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKExecuteTransactionOutputPin Failed;

	void DoExecuteTransaction(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnExecuteTransactionResponse(FOpenfortOpenfortSDKResult Result);
};