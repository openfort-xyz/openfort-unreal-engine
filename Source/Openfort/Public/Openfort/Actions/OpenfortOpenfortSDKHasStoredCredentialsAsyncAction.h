#pragma once

#include "CoreMinimal.h"
#include "OpenfortBlueprintAsyncAction.h"

#include "OpenfortOpenfortSDKHasStoredCredentialsAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKHasStoredCredentialsOutputPin, FString, ErrorMessage);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKHasStoredCredentialsAsyncAction *HasStoredCredentials(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	void DoHasStoredCredentials(TWeakObjectPtr<class UOpenfortJSConnector> JSGetConnector);
	void OnHasStoredCredentialsResponse(struct FOpenfortOpenfortSDKResult Result);

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "True"))
	FOpenfortSDKHasStoredCredentialsOutputPin OnTrue;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "False"))
	FOpenfortSDKHasStoredCredentialsOutputPin OnFalse;
};
