#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JsonObjectConverter.h"
#include "Openfort/OpenfortJSConnector.h"
#include "Openfort/OpenfortDataTypes.h"
#include "Openfort/OpenfortRequests.h"
#include "OpenfortOpenfortSDK.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthEventDelegate, EOpenfortAuthEvent, AuthEvent);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOpenfortOpenfortSDKResponseDelegate, const FOpenfortOpenfortSDKResult &, Result);

UCLASS()
class OPENFORT_API UOpenfortOpenfortSDK : public UObject
{
	GENERATED_BODY()

public:
	static UOpenfortOpenfortSDK *GetInstance();

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void Initialize(const FString &PublishableKey, const FString &ShieldPublishableKey, const FString &ShieldEncryptionKey, bool ShieldDebug, const FString &BackendUrl, const FString &IframeUrl, const FString &ShieldUrl, const FString &Deeplink, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void SetCallTimeout(int32 Milliseconds);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void LogInWithEmailPassword(const FString &Email, const FString &Password, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void SignUpWithEmailPassword(const FString &Email, const FString &Password, const FString &Name, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void LinkEmailPassword(const FLinkEmailPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void UnlinkEmailPassword(const FUnlinkEmailPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void RequestResetPassword(const FResetPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void ResetPassword(const FResetPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void RequestEmailVerification(const FRequestEmailVerificationRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void VerifyEmail(const FVerifyEmailRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void InitOAuth(const FOAuthInitRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void AuthenticateWithOAuth(const FOAuthInitRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void UnlinkOAuth(const FUnlinkOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void PoolOAuth(const FPoolOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void InitLinkOAuth(const FInitLinkOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void AuthenticateWithThirdPartyProvider(const FThirdPartyProviderRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void InitSiwe(const FInitSiweRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void AuthenticateWithSiwe(const FAuthenticateWithSiweRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void LinkWallet(const FLinkWalletRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void UnlinkWallet(const FUnlinkWalletRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void StoreCredentials(const FAuthCredentialsRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void GetUser(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void Logout(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void GetAccessToken(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void ValidateAndRefreshToken(const FValidateAndRefreshTokenRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void SendSignatureTransactionIntentRequest(const FSignatureTransactionIntentRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void SignMessage(const FSignMessageRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void SignTypedData(const FSignTypedDataRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void SendSignatureSessionRequest(const FRegisterSessionRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void GetEmbeddedState(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void GetEthereumProvider(const FEthereumProviderRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void ConfigureEmbeddedSigner(const FEmbeddedSignerRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void ClearCache(bool IncludeDiskFiles);

	UFUNCTION(BlueprintCallable, Category = "Openfort")
	void ClearStorage();

	UPROPERTY(BlueprintAssignable, Category = "Openfort")
	FOnAuthEventDelegate OnAuthEvent;

protected:
	void Setup(TWeakObjectPtr<class UOpenfortJSConnector> Connector);
	void ReinstateConnection(FOpenfortJSResponse Response);

	TWeakObjectPtr<UOpenfortJSConnector> JSConnector;
	FOpenfortOpenfortSDKInitData InitData;
	FDelegateHandle BridgeReadyHandle;
	TMap<FString, FOpenfortOpenfortSDKResponseDelegate> ResponseDelegates;

	bool CheckIsInitialized(const FString &Action, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate) const;
	void CallJS(const FString &Action, const FString &Data, const FOpenfortOpenfortSDKResponseDelegate &ClientResponseDelegate, const FOpenfortJSResponseDelegate &HandleJSResponse, const bool bCheckInitialized = true);
	TOptional<FOpenfortOpenfortSDKResponseDelegate> GetResponseDelegate(const FOpenfortJSResponse &Response);

private:
	static UOpenfortOpenfortSDK *Instance;

	bool bIsInitialized;
	bool bIsLoggedIn;
	FString Deeplink;

	void OnDeepLinkActivated(const FString &Url);
	void BroadcastAuthEvent(EOpenfortAuthEvent AuthEvent);

	UOpenfortOpenfortSDK();
	virtual void BeginDestroy() override;
};