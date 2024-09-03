#pragma once

#include "CoreMinimal.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "OpenfortBlueprintAsyncAction.h"
#include "OpenfortOpenfortSDKLogoutAsyncAction.generated.h"

/**
 *
 */
// UCLASS(meta = (HasDedicatedAsyncNode))
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDKLogoutAsyncAction : public UOpenfortBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenfortSDKLogoutOutPin, FString, Message);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Openfort")
	static UOpenfortOpenfortSDKLogoutAsyncAction *Logout(UObject *WorldContextObject);

	virtual void Activate() override;

private:
	void DoLogout(TWeakObjectPtr<class UOpenfortJSConnector> JSConnector);
	void OnLogoutResponse(FOpenfortOpenfortSDKResult Result) const;

private:
	bool bDoHardLogout = true;

	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKLogoutOutPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FOpenfortSDKLogoutOutPin OnFailure;
};
