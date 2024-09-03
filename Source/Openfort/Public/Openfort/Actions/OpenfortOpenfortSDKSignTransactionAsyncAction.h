#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKSignTransactionAsyncAction.generated.h"

UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKSignTransactionAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKSignTransactionOutputPin, FString, ErrorMessage, const struct FTransactionIntentResponse &, Response);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKSignTransactionAsyncAction *SignTransaction(UObject *WorldContextObject, const FSignatureTransactionIntentRequest &Request);

	virtual void Activate() override;

private:
	FSignatureTransactionIntentRequest TransactionRequest;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKSignTransactionOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKSignTransactionOutputPin Failed;

	void DoSignTransaction(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnSignTransactionResponse(FOpenfortOpenfortSDKResult Result);
};