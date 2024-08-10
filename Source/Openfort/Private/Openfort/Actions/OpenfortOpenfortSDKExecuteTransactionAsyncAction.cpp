// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortOpenfortSDKExecuteTransactionAsyncAction.h"

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Openfort/Misc/OpenfortLogging.h"

UOpenfortOpenfortSDKExecuteTransactionAsyncAction *UOpenfortOpenfortSDKExecuteTransactionAsyncAction::ExecuteTransaction(UObject *WorldContextObject, const FOpenfortExecuteTransactionRequest &Request)
{
	UOpenfortOpenfortSDKExecuteTransactionAsyncAction *OpenfortSDKExecuteTransactionBlueprintNode = NewObject<UOpenfortOpenfortSDKExecuteTransactionAsyncAction>();
	OpenfortSDKExecuteTransactionBlueprintNode->WorldContextObject = WorldContextObject;
	OpenfortSDKExecuteTransactionBlueprintNode->TransactionRequest = Request;
	return OpenfortSDKExecuteTransactionBlueprintNode;
}

void UOpenfortOpenfortSDKExecuteTransactionAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "EVM Execute Transaction failed due to missing world or world "
					  "context object.";
		OPENFORT_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UOpenfortOpenfortSDKExecuteTransactionAsyncAction::DoExecuteTransaction);
}

void UOpenfortOpenfortSDKExecuteTransactionAsyncAction::DoExecuteTransaction(TWeakObjectPtr<UOpenfortJSConnector> JSConnector)
{
	// Get OpenfortSDK
	auto OpenfortSDK = GetSubsystem()->GetOpenfortSDK();
	// Run ExecuteTransaction
	OpenfortSDK->ExecuteTransaction(TransactionRequest, UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDKExecuteTransactionAsyncAction::OnExecuteTransactionResponse));
}

void UOpenfortOpenfortSDKExecuteTransactionAsyncAction::OnExecuteTransactionResponse(FOpenfortOpenfortSDKResult Result)
{
	if (Result.Success)
	{
		OPENFORT_LOG("EVM Send Transaction success")
		TransactionSent.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		OPENFORT_LOG("EVM Send Transaction failed")
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}