// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
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

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKConfigureEmbeddedSignerOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKConfigureEmbeddedSignerOutputPin Failed;

	FEmbeddedSignerRequest ShieldConfig;
};
