// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKRequestWalletSessionKeyAsyncAction.generated.h"

/**
 * Async action blueprint node to request a session key.
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKRequestWalletSessionKey : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKRequestWalletSessionKeyOutputPin, FString, ErrorMessage);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKRequestWalletSessionKey *RequestWalletSessionKey(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKRequestWalletSessionKeyOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKRequestWalletSessionKeyOutputPin Failed;

	void DoRequestWalletSessionKey(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnRequestWalletSessionKeyResponse(FOpenfortOpenfortSDKResult Result);
};
