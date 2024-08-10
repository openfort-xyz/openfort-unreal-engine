// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKGetIdTokenAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKGetIdTokenAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKGetIdTokenOutputPin, FString, ErrorMessage, FString, IdToken);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKGetIdTokenAsyncAction *GetIdToken(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	void DoGetIdToken(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnGetIdTokenResponse(FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKGetIdTokenOutputPin GotIdToken;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKGetIdTokenOutputPin Failed;
};
