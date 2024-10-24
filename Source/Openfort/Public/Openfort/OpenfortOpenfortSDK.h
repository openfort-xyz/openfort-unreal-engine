#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Misc/EngineVersion.h"
#include "Engine/TimerHandle.h"
#include "Runtime/Core/Public/HAL/Platform.h"
#include "UObject/Object.h"
#include "Openfort/OpenfortJSConnector.h"
#include "Openfort/OpenfortDataTypes.h"
#include "Openfort/OpenfortRequests.h"
#include "OpenfortOpenfortSDK.generated.h"

template <typename UStructType>
FString UStructToJsonString(const UStructType &InStruct)
{
	FString OutString;
	FJsonObjectConverter::UStructToJsonObjectString(InStruct, OutString, 0, 0, 0, nullptr, false);
	return OutString;
}

template <typename UStructType>
TOptional<UStructType> JsonObjectToUStruct(const TSharedPtr<FJsonObject> &JsonObject)
{
	if (!JsonObject.IsValid())
	{
		return TOptional<UStructType>();
	}

	// Parse the JSON
	UStructType StructInstance;
	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &StructInstance, 0, 0))
	{
		OPENFORT_ERR("Could not parse response from JavaScript into the expected Ustruct");
		return TOptional<UStructType>();
	}
	return StructInstance;
}

/**
 * Openfort sdk object
 */
UCLASS()
class OPENFORT_API UOpenfortOpenfortSDK : public UObject
{
	GENERATED_BODY()
	friend class UOpenfortSubsystem;

public:
	DECLARE_MULTICAST_DELEGATE(FOnOpenfortSDKReadyDelegate);

	DECLARE_DELEGATE_OneParam(FOpenfortOpenfortSDKResponseDelegate, FOpenfortOpenfortSDKResult);

#if PLATFORM_ANDROID
	void HandleDeepLink(FString DeepLink) const;
	void HandleCustomTabsDismissed(FString Url);
#elif PLATFORM_IOS | PLATFORM_MAC
	void HandleDeepLink(NSString *sDeepLink) const;
#endif

	void Initialize(const FOpenfortOpenfortSDKInitData &InitData, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void LogInWithEmailPassword(const FString &Email, const FString &Password, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void SignUpWithEmailPassword(const FString &Email, const FString &Password, const FString &Name, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void LinkEmailPassword(const FLinkEmailPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void UnlinkEmailPassword(const FUnlinkEmailPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void RequestResetPassword(const FResetPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void ResetPassword(const FResetPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void RequestEmailVerification(const FRequestEmailVerificationRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void VerifyEmail(const FVerifyEmailRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void InitOAuth(const FOAuthInitRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void AuthenticateWithOAuth(const FOAuthInitRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void UnlinkOAuth(const FUnlinkOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void PoolOAuth(const FPoolOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void InitLinkOAuth(const FInitLinkOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void AuthenticateWithThirdPartyProvider(const FThirdPartyProviderRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void InitSiwe(const FInitSiweRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void AuthenticateWithSiwe(const FAuthenticateWithSiweRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void LinkWallet(const FLinkWalletRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void UnlinkWallet(const FUnlinkWalletRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void StoreCredentials(const FAuthCredentialsRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void GetUser(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void Logout(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void GetAccessToken(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void ValidateAndRefreshToken(const FValidateAndRefreshTokenRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void SendSignatureTransactionIntentRequest(const FSignatureTransactionIntentRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void SignMessage(const FSignMessageRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void SignTypedData(const FSignTypedDataRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void SendSignatureSessionRequest(const FRegisterSessionRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void GetEmbeddedState(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void GetEthereumProvider(const FEthereumProviderRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void ConfigureEmbeddedSigner(const FEmbeddedSignerRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate);
	void ClearCache(bool IncludeDiskFiles);
	void ClearStorage();

	UPROPERTY(BlueprintAssignable, Category = "Openfort")
	FOnAuthEventDelegate OnAuthEvent;

protected:
	void Setup(TWeakObjectPtr<class UOpenfortJSConnector> Connector);
	void ReinstateConnection(FOpenfortJSResponse Response);

#if PLATFORM_ANDROID
	DECLARE_DELEGATE(FOpenfortOpenfortSDKOnDismissedDelegate);

	FOpenfortOpenfortSDKOnDismissedDelegate OnDismissed;
#endif

	TWeakObjectPtr<UOpenfortJSConnector> JSConnector;
	FDelegateHandle BridgeReadyHandle;
	TMap<FString, FOpenfortOpenfortSDKResponseDelegate> ResponseDelegates;
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	DECLARE_DELEGATE_OneParam(FOpenfortOpenfortSDKHandleDeepLinkDelegate, FString);

	FOpenfortOpenfortSDKHandleDeepLinkDelegate OnHandleDeepLink;
	FOpenfortOpenfortSDKResponseDelegate DeepResponseDelegate;
#endif

	// Ensures that OpenfortSDK has been initialized before calling JS
	bool CheckIsInitialized(const FString &Action, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate) const;
	// Calls JS with the given Action and Data, and registers the given
	// ResponseDelegate to be called when JS responds
	void CallJS(const FString &Action, const FString &Data, const FOpenfortOpenfortSDKResponseDelegate &ClientResponseDelegate, const FOpenfortJSResponseDelegate &HandleJSResponse, const bool bCheckInitialized = true);
	// Pulls the ResponseDelegate from the ResponseDelegates map and returns it
	TOptional<FOpenfortOpenfortSDKResponseDelegate> GetResponseDelegate(const FOpenfortJSResponse &Response);
	void OnInitializeResponse(FOpenfortJSResponse Response);
	void OnLoginResponse(FOpenfortJSResponse Response);
	void OnSignupResponse(FOpenfortJSResponse Response);
	void OnLinkEmailPasswordResponse(FOpenfortJSResponse Response);
	void OnUnlinkEmailPasswordResponse(FOpenfortJSResponse Response);
	void OnRequestResetPasswordResponse(FOpenfortJSResponse Response);
	void OnResetPasswordResponse(FOpenfortJSResponse Response);
	void OnRequestEmailVerificationResponse(FOpenfortJSResponse Response);
	void OnVerifyEmailResponse(FOpenfortJSResponse Response);
	void OnUnlinkOAuthResponse(FOpenfortJSResponse Response);
	void OnPoolOAuthResponse(FOpenfortJSResponse Response);
	void OnInitLinkOAuthResponse(FOpenfortJSResponse Response);
	void OnAuthenticateWithThirdPartyProviderResponse(FOpenfortJSResponse Response);
	void OnInitSiweResponse(FOpenfortJSResponse Response);
	void OnAuthenticateWithSiweResponse(FOpenfortJSResponse Response);
	void OnLinkWalletResponse(FOpenfortJSResponse Response);
	void OnUnlinkWalletResponse(FOpenfortJSResponse Response);
	void OnStoreCredentialsResponse(FOpenfortJSResponse Response);
	void OnGetUserResponse(FOpenfortJSResponse Response);
	void OnLogoutResponse(FOpenfortJSResponse Response);
	void OnGetAccessTokenResponse(FOpenfortJSResponse Response);
	void OnValidateAndRefreshTokenResponse(FOpenfortJSResponse Response);
	void OnSendSignatureTransactionIntentRequestResponse(FOpenfortJSResponse Response);
	void OnSignMessageResponse(FOpenfortJSResponse Response);
	void OnSignTypedDataResponse(FOpenfortJSResponse Response);
	void OnSendSignatureSessionRequestResponse(FOpenfortJSResponse Response);
	void OnGetEmbeddedStateResponse(FOpenfortJSResponse Response);
	void OnGetEthereumProviderResponse(FOpenfortJSResponse Response);
	void OnAuthenticateWithOAuthResponse(FOpenfortJSResponse Response, const FString &RedirectTo);
	void OnConfigureEmbeddedSignerResponse(FOpenfortJSResponse Response);
	void OnInitOAuthResponse(FOpenfortJSResponse Response);

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	void OnDeepLinkActivated(FString DeepLink);
	void CompleteAuthenticationFlow(FString Url);
#endif
#if PLATFORM_ANDROID
	void HandleOnLoginDismissed();
	void CallJniStaticVoidMethod(JNIEnv *Env, const jclass Class, jmethodID Method, ...);
	void LaunchAndroidUrl(FString Url);
#endif
private:
	FString RedirectUri;
	FDelegateHandle DeepLinkHandle;
	FTimerHandle PoolingTimerHandle;
	bool bIsInitialized;
	bool bIsLoggedIn;
	FOpenfortOpenfortSDKResponseDelegate MainResponseDelegate;
};