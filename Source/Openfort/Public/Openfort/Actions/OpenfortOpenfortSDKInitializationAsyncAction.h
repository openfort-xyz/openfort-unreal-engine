// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"

#include "OpenfortOpenfortSDKInitializationAsyncAction.generated.h"

/**
 * Async action to instantiate
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKInitializationAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKInitializationOutputPin, FString, Message);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKInitializationAsyncAction *InitializeOpenfortSDK(UObject *WorldContextObject, const FString &ClientID, const FString &RedirectUri, const FString &LogoutUri);

	virtual void Activate() override;

private:
	FString ClientId;
	FString RedirectUri;
	FString LogoutUri;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKInitializationOutputPin Initialized;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKInitializationOutputPin Failed;

	void DoInit(TWeakObjectPtr<class UOpenfortJSConnector> JSConnector);
	void OnInitialized(FOpenfortOpenfortSDKResult Result);
};
