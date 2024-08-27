// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortAuthenticateAsyncAction.generated.h"

UCLASS()
class OPENFORT_API UOpenfortConnectionAsyncActions : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKAuthenticateOutputPin, FString, ErrorMessage);

public:
	/**
	 * Authenticate with OpenfortSDK using PKCE flow.
	 *
	 * @param    WorldContextObject    World context
	 *
	 * @return    A reference to the object represented by this node
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortConnectionAsyncActions *AuthenticateWithOAuth(UObject *WorldContextObject);

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
};