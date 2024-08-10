// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKGetAccessTokenAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKGetAccessTokenAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKGetAccessTokenOutputPin, FString, ErrorMessage, FString, AccessToken);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKGetAccessTokenAsyncAction *GetAccessToken(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	void DoGetAccessToken(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnGetAccessTokenResponse(FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKGetAccessTokenOutputPin GotAccessToken;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKGetAccessTokenOutputPin Failed;
};
