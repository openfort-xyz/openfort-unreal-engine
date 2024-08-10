// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortAuthenticateAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortConnectionAsyncActions : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKConnectOutputPin, FString, ErrorMessage);

public:
	/**
	 * Log into OpenfortSDK using Device Code Authorisation.
	 *
	 * @param	WorldContextObject	World context
	 * @param	UseCachedSession	Whether to use stored credentials for relogin
	 *
	 * @return	A reference to the object represented by this node
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortConnectionAsyncActions *Authenticate(UObject *WorldContextObject, bool UseCachedSession = false);

	/**
	 * Log into OpenfortSDK using PKCE
	 *
	 * @param	WorldContextObject	World context
	 *
	 * @return	A reference to the object represented by this node
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortConnectionAsyncActions *AuthenticatePKCE(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	FOpenfortOpenfortSDKInitDeviceFlowData InitDeviceFlowData;

	void DoConnect(TWeakObjectPtr<class UOpenfortJSConnector> JSConnector);
	void OnConnect(FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKConnectOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKConnectOutputPin Failed;

	bool bUseCachedSession = false;
	bool bIsAuthenticate = false;
	bool bIsPKCE = false;
};
