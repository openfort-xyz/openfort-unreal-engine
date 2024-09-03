// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKGetUserAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKGetUserAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKGetUserOutputPin, FString, ErrorMessage, FString, User);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKGetUserAsyncAction *GetUser(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	void DoGetUser(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnGetUserResponse(FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKGetUserOutputPin GotUser;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKGetUserOutputPin Failed;
};
