// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpenfortSDKAuthenticateWithThirdPartyProviderOutputPin, FString, ErrorMessage, FString, User);

public:
	/**
	 * Authenticate with a third party provider.
	 *
	 * @param    WorldContextObject    World context
	 * @param    ThirdPartyProviderRequest    The request to authenticate with a third party provider
	 *
	 * @return    A reference to the object represented by this node
	 */

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKAuthenticateWithThirdPartyProviderAsyncAction *AuthenticateWithThirdPartyProvider(UObject *WorldContextObject, const FThirdPartyProviderRequest &ThirdPartyProviderRequest);

	virtual void Activate() override;

private:
	void DoAuthenticateWithThirdPartyProvider(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnAuthenticateWithThirdPartyProviderResponse(FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKAuthenticateWithThirdPartyProviderOutputPin GotUser;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKAuthenticateWithThirdPartyProviderOutputPin Failed;

	FThirdPartyProviderRequest ThirdPartyProviderRequest;
};
