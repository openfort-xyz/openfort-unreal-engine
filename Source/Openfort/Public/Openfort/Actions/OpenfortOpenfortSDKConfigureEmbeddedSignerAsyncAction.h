// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "Interfaces/IHttpRequest.h"
#include "OpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKConfigureEmbeddedSignerOutputPin, FString, Message, FString, ErrorMessage);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKConfigureEmbeddedSignerAsyncAction *ConfigureEmbeddedSigner(UObject *WorldContextObject, const FEmbeddedSignerRequest &ShieldConfig);

	virtual void Activate() override;

private:
	void DoConfigureEmbeddedSigner(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnConfigureEmbeddedSignerResponse(FOpenfortOpenfortSDKResult Result);
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKConfigureEmbeddedSignerOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKConfigureEmbeddedSignerOutputPin Failed;

	FEmbeddedSignerRequest ShieldConfig;
	FSignatureTransactionIntentRequest TransactionRequest;
	FString AccessToken;
};