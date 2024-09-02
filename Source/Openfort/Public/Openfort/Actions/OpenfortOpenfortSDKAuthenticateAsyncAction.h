// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKAuthenticateAsyncAction.generated.h"

UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKAuthenticateAsyncActions : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKAuthenticateOutputPin, FString, ErrorMessage);

public:
	/**
	 * Authenticate with OpenfortSDK.
	 *
	 * @param    WorldContextObject    World context
	 * @param    Provider               The OAuth provider to use for authentication
	 * @param    Options                Additional options for the OAuth request
	 * @param    bUsePooling            Whether to use pooling for the OAuth request
	 *
	 * @return    A reference to the object represented by this node
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKAuthenticateAsyncActions *AuthenticateWithOAuth(UObject *WorldContextObject, EOAuthProvider Provider, const FOAuthInitRequestOptions &Options, bool bUsePooling);

	virtual void Activate() override;

private:
	void DoAuthenticate(TWeakObjectPtr<class UOpenfortJSConnector> JSConnector);
	void OnAuthenticate(FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKAuthenticateOutputPin Success;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKAuthenticateOutputPin Failed;

	bool bIsAuthenticate = false;

	UPROPERTY()
	UObject *WorldContextObject;

	EOAuthProvider Provider;
	FOAuthInitRequestOptions Options;
	bool bUsePooling;
};