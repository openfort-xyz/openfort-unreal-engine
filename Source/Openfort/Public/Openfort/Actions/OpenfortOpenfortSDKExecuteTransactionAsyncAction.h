#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "HttpModule.h"

#include "OpenfortOpenfortSDKExecuteTransactionAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOpenfortSDKExecuteTransactionOutputPin, FString, ErrorMessage, FString, TransactionIntentId, FString, UserOperationHash);

UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKExecuteTransactionAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "Openfort"))
	static UOpenfortOpenfortSDKExecuteTransactionAsyncAction *ExecuteTransaction(UObject *WorldContextObject, const FString &AccessToken);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKExecuteTransactionOutputPin TransactionSent;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKExecuteTransactionOutputPin Failed;

private:
	UPROPERTY()
	UObject *WorldContextObject;

	FString AccessToken;

	void DoExecuteTransaction();
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
};