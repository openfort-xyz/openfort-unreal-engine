#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "Openfort/OpenfortJSConnector.h"
#include "JsonObjectConverter.h"
#include "Openfort/OpenfortSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Policies/CondensedJsonPrintPolicy.h"

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
#include "GenericPlatform/GenericPlatformHttp.h"
#endif

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/OpenfortAndroidJNI.h"
#elif PLATFORM_IOS
#include "IOS/OpenfortIOS.h"
#elif PLATFORM_MAC
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Mac/OpenfortMac.h"
#endif

void UOpenfortOpenfortSDK::Initialize(const FOpenfortOpenfortSDKInitData &InitData, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	check(JSConnector.IsValid());

	CallJS(OpenfortOpenfortSDKAction::INIT, InitData.ToJsonString(), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitializeResponse), false);
}

void UOpenfortOpenfortSDK::LogInWithEmailPassword(const FString &Email, const FString &Password, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	FLoginEmailPasswordRequest Request;
	Request.Email = Email;
	Request.Password = Password;

	CallJS(OpenfortOpenfortSDKAction::LOGIN_WITH_EMAIL_PASSWORD, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnLoginResponse));
}

void UOpenfortOpenfortSDK::SignUpWithEmailPassword(const FString &Email, const FString &Password, const FString &Name, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	FSignupEmailPasswordRequest Request;
	Request.Email = Email;
	Request.Password = Password;
	Request.Options.Data.Name = Name;

	CallJS(OpenfortOpenfortSDKAction::SIGNUP_WITH_EMAIL_PASSWORD, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnSignupResponse));
}

void UOpenfortOpenfortSDK::LinkEmailPassword(const FLinkEmailPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::LINK_EMAIL_PASSWORD, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnLinkEmailPasswordResponse));
}

void UOpenfortOpenfortSDK::UnlinkEmailPassword(const FUnlinkEmailPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::UNLINK_EMAIL_PASSWORD, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnUnlinkEmailPasswordResponse));
}

void UOpenfortOpenfortSDK::RequestResetPassword(const FResetPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::REQUEST_RESET_PASSWORD, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnRequestResetPasswordResponse));
}

void UOpenfortOpenfortSDK::ResetPassword(const FResetPasswordRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::RESET_PASSWORD, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnResetPasswordResponse));
}

void UOpenfortOpenfortSDK::RequestEmailVerification(const FRequestEmailVerificationRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::REQUEST_EMAIL_VERIFICATION, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnRequestEmailVerificationResponse));
}

void UOpenfortOpenfortSDK::VerifyEmail(const FVerifyEmailRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::VERIFY_EMAIL, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnVerifyEmailResponse));
}

void UOpenfortOpenfortSDK::AuthenticateWithOAuth(const FOAuthInitRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	MainResponseDelegate = ResponseDelegate;

#if PLATFORM_ANDROID || PLATFORM_IOS || PLATFORM_MAC
	CallJS(OpenfortOpenfortSDKAction::INIT_OAUTH, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitOAuthResponse));
#else
	FString InitOAuthResponse = InitOAuth(Request);
	OnInitOAuthResponse(FOpenfortJSResponse(InitOAuthResponse));
#endif
}

void UOpenfortOpenfortSDK::InitOAuth(const FOAuthInitRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::INIT_OAUTH, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitOAuthResponse));
}

void UOpenfortOpenfortSDK::UnlinkOAuth(const FUnlinkOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::UNLINK_OAUTH, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnUnlinkOAuthResponse));
}

void UOpenfortOpenfortSDK::PoolOAuth(const FPoolOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::POOL_OAUTH, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnPoolOAuthResponse));
}

void UOpenfortOpenfortSDK::InitLinkOAuth(const FInitLinkOAuthRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::INIT_LINK_OAUTH, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitLinkOAuthResponse));
}

void UOpenfortOpenfortSDK::AuthenticateWithThirdPartyProvider(const FThirdPartyProviderRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::AUTHENTICATE_WITH_THIRD_PARTY_PROVIDER, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnAuthenticateWithThirdPartyProviderResponse));
}

void UOpenfortOpenfortSDK::InitSiwe(const FInitSiweRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::INIT_SIWE, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitSiweResponse));
}

void UOpenfortOpenfortSDK::AuthenticateWithSiwe(const FAuthenticateWithSiweRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::AUTHENTICATE_WITH_SIWE, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnAuthenticateWithSiweResponse));
}

void UOpenfortOpenfortSDK::LinkWallet(const FLinkWalletRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::LINK_WALLET, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnLinkWalletResponse));
}

void UOpenfortOpenfortSDK::UnlinkWallet(const FUnlinkWalletRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::UNLINK_WALLET, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnUnlinkWalletResponse));
}

void UOpenfortOpenfortSDK::StoreCredentials(const FAuthCredentialsRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::STORE_CREDENTIALS, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnStoreCredentialsResponse));
}

void UOpenfortOpenfortSDK::GetUser(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::GET_USER, TEXT(""), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnGetUserResponse));
}

void UOpenfortOpenfortSDK::Logout(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::LOGOUT, TEXT(""), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnLogoutResponse));
}

void UOpenfortOpenfortSDK::GetAccessToken(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::GET_ACCESS_TOKEN, TEXT(""), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnGetAccessTokenResponse));
}

void UOpenfortOpenfortSDK::ValidateAndRefreshToken(const FValidateAndRefreshTokenRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::VALIDATE_AND_REFRESH_TOKEN, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnValidateAndRefreshTokenResponse));
}

void UOpenfortOpenfortSDK::SendSignatureTransactionIntentRequest(const FSignatureTransactionIntentRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::SEND_SIGNATURE_TRANSACTION_INTENT_REQUEST, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnSendSignatureTransactionIntentRequestResponse));
}

void UOpenfortOpenfortSDK::SignMessage(const FSignMessageRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::SIGN_MESSAGE, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnSignMessageResponse));
}

void UOpenfortOpenfortSDK::SignTypedData(const FSignTypedDataRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::SIGN_TYPED_DATA, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnSignTypedDataResponse));
}

void UOpenfortOpenfortSDK::SendSignatureSessionRequest(const FRegisterSessionRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::SEND_SIGNATURE_SESSION_REQUEST, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnSendSignatureSessionRequestResponse));
}

void UOpenfortOpenfortSDK::GetEmbeddedState(const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::GET_EMBEDDED_STATE, TEXT(""), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnGetEmbeddedStateResponse));
}

void UOpenfortOpenfortSDK::GetEthereumProvider(const FEthereumProviderRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::GET_ETHEREUM_PROVIDER, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnGetEthereumProviderResponse));
}

void UOpenfortOpenfortSDK::ConfigureEmbeddedSigner(const FEmbeddedSignerRequest &Request, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	CallJS(OpenfortOpenfortSDKAction::CONFIGURE_EMBEDDED_SIGNER, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnConfigureEmbeddedSignerResponse));
}

void UOpenfortOpenfortSDK::OnInitializeResponse(FOpenfortJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		if (Response.success)
		{
			OPENFORT_LOG("Identity initialization succeeded.")
		}
		else
		{
			OPENFORT_ERR("Identity initialization failed.")
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}
		ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{Response.success, Msg, Response});
	}
}

void UOpenfortOpenfortSDK::OnLoginResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnLoginResponse
}

void UOpenfortOpenfortSDK::OnSignupResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnSignupResponse
}

void UOpenfortOpenfortSDK::OnLinkEmailPasswordResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnLinkEmailPasswordResponse
}

void UOpenfortOpenfortSDK::OnUnlinkEmailPasswordResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnUnlinkEmailPasswordResponse
}

void UOpenfortOpenfortSDK::OnRequestResetPasswordResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnRequestResetPasswordResponse
}

void UOpenfortOpenfortSDK::OnResetPasswordResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnResetPasswordResponse
}

void UOpenfortOpenfortSDK::OnRequestEmailVerificationResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnRequestEmailVerificationResponse
}

void UOpenfortOpenfortSDK::OnVerifyEmailResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnVerifyEmailResponse
}

void UOpenfortOpenfortSDK::OnInitOAuthResponse(FOpenfortJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		if (!Response.success)
		{
			FString ErrorMsg = Response.Error.IsSet() ? Response.Error->ToString() : Response.JsonObject->GetStringField(TEXT("error"));
			OPENFORT_ERR("OAuth initialization failed: %s", *ErrorMsg);
			ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{false, ErrorMsg, Response});
			return;
		}

		FString AuthUrl, AuthKey;
		if (!Response.JsonObject->TryGetStringField(TEXT("url"), AuthUrl) || !Response.JsonObject->TryGetStringField(TEXT("key"), AuthKey))
		{
			OPENFORT_ERR("Invalid OAuth init response");
			ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{false, TEXT("Invalid OAuth init response"), Response});
			return;
		}

		LaunchAuthUrl(AuthUrl);
		PoolOAuth(AuthKey);
	}
}

void UOpenfortOpenfortSDK::OnUnlinkOAuthResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnUnlinkOAuthResponse
}

void UOpenfortOpenfortSDK::OnPoolOAuthResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnPoolOAuthResponse
}

void UOpenfortOpenfortSDK::OnInitLinkOAuthResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnInitLinkOAuthResponse
}

void UOpenfortOpenfortSDK::OnAuthenticateWithThirdPartyProviderResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnAuthenticateWithThirdPartyProviderResponse
}

void UOpenfortOpenfortSDK::OnInitSiweResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnInitSiweResponse
}

void UOpenfortOpenfortSDK::OnAuthenticateWithSiweResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnAuthenticateWithSiweResponse
}

void UOpenfortOpenfortSDK::OnLinkWalletResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnLinkWalletResponse
}

void UOpenfortOpenfortSDK::OnUnlinkWalletResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnUnlinkWalletResponse
}

void UOpenfortOpenfortSDK::OnStoreCredentialsResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnStoreCredentialsResponse
}

void UOpenfortOpenfortSDK::OnGetUserResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnGetUserResponse
}

void UOpenfortOpenfortSDK::OnLogoutResponse(FOpenfortJSResponse Response)
{
	auto ResponseDelegate = GetResponseDelegate(Response);

	if (!ResponseDelegate)
	{
		return;
	}

	FString Message;

	if (!Response.success)
	{
		Message = Response.Error.IsSet() ? Response.Error->ToString() : Response.JsonObject->GetStringField(TEXT("error"));

		OPENFORT_ERR("%s", *Message)
		ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{Response.success, Message, Response});

		return;
	}

	Message = "Logged out without clearing browser session";

	OPENFORT_LOG("%s", *Message)
	ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{true, Message});

	return;

}

void UOpenfortOpenfortSDK::OnGetAccessTokenResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnGetAccessTokenResponse
}

void UOpenfortOpenfortSDK::OnValidateAndRefreshTokenResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnValidateAndRefreshTokenResponse
}

void UOpenfortOpenfortSDK::OnSendSignatureTransactionIntentRequestResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnSendSignatureTransactionIntentRequestResponse
}

void UOpenfortOpenfortSDK::OnSignMessageResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnSignMessageResponse
}

void UOpenfortOpenfortSDK::OnSignTypedDataResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnSignTypedDataResponse
}

void UOpenfortOpenfortSDK::OnSendSignatureSessionRequestResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnSendSignatureSessionRequestResponse
}

void UOpenfortOpenfortSDK::OnGetEmbeddedStateResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnGetEmbeddedStateResponse
}

void UOpenfortOpenfortSDK::OnGetEthereumProviderResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnGetEthereumProviderResponse
}

void UOpenfortOpenfortSDK::OnConfigureEmbeddedSignerResponse(FOpenfortJSResponse Response)
{
	// Implementation for OnConfigureEmbeddedSignerResponse
}

void UOpenfortOpenfortSDK::BeginDestroy

	void
	UOpenfortOpenfortSDK::LaunchAuthUrl(const FString &Url)
{
#if PLATFORM_ANDROID || PLATFORM_IOS || PLATFORM_MAC
	// Register for deep link handling
	DeepLinkHandle = FPlatformMisc::OnDeepLinkActivated().AddUObject(this, &UOpenfortOpenfortSDK::HandleDeepLink);
#endif

	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}

void UOpenfortOpenfortSDK::PoolOAuth(const FString &Key)
{
	GetWorld()->GetTimerManager().SetTimer(PoolingTimerHandle, [this, Key]()
										   {
        FPoolOAuthRequest PoolRequest;
        PoolRequest.Key = Key;

        CallJS(OpenfortOpenfortSDKAction::POOL_OAUTH, UStructToJsonString(PoolRequest), MainResponseDelegate, 
            FOpenfortJSResponseDelegate::CreateLambda([this](FOpenfortJSResponse Response)
            {
                if (Response.success)
                {
                    bIsLoggedIn = true;
                    GetWorld()->GetTimerManager().ClearTimer(PoolingTimerHandle);
                    MainResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{true, TEXT("Authentication successful"), Response});
                }
                else
                {
                    FString ErrorMsg = Response.Error.IsSet() ? Response.Error->ToString() : TEXT("Authentication failed");
                    OPENFORT_ERR("%s", *ErrorMsg);
                    // Don't clear the timer here, allow it to continue polling
                }
            })); }, 2.0f, true); // Poll every 2 seconds
}

void UOpenfortOpenfortSDK::HandleDeepLink(const FString &DeepLink)
{
	if (DeepLink.StartsWith(RedirectUri))
	{
		CompleteAuthenticationFlow(DeepLink);

		// Unregister deep link handler
		FPlatformMisc::OnDeepLinkActivated().Remove(DeepLinkHandle);
	}
}

void UOpenfortOpenfortSDK::CompleteAuthenticationFlow(const FString &Uri)
{
	FString AccessToken, RefreshToken;
	TMap<FString, FString> Params;
	FGenericPlatformHttp::ParseURL(*Uri, RedirectUri, Params);

	if (!Params.Contains(TEXT("access_token")) || !Params.Contains(TEXT("refresh_token")))
	{
		OPENFORT_ERR("Invalid authentication response");
		MainResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{false, TEXT("Invalid authentication response")});
		return;
	}

	AccessToken = Params[TEXT("access_token")];
	RefreshToken = Params[TEXT("refresh_token")];

	FAuthCredentialsRequest CredentialsRequest;
	CredentialsRequest.AccessToken = AccessToken;
	CredentialsRequest.RefreshToken = RefreshToken;

	CallJS(OpenfortOpenfortSDKAction::STORE_CREDENTIALS, UStructToJsonString(CredentialsRequest), MainResponseDelegate,
		   FOpenfortJSResponseDelegate::CreateLambda([this](FOpenfortJSResponse Response)
													 {
            if (Response.success)
            {
                bIsLoggedIn = true;
                MainResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{true, TEXT("Authentication successful"), Response});
            }
            else
            {
                FString ErrorMsg = Response.Error.IsSet() ? Response.Error->ToString() : TEXT("Failed to store credentials");
                OPENFORT_ERR("%s", *ErrorMsg);
                MainResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{false, ErrorMsg, Response});
            } }));
}
