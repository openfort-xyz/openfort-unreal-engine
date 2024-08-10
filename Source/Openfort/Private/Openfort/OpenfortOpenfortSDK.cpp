#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "Openfort/OpenfortJSConnector.h"
#include "JsonObjectConverter.h"
#include "Openfort/OpenfortSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Policies/CondensedJsonPrintPolicy.h"

UOpenfortOpenfortSDK *UOpenfortOpenfortSDK::Instance = nullptr;

UOpenfortOpenfortSDK *UOpenfortOpenfortSDK::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<UOpenfortOpenfortSDK>();
		Instance->AddToRoot(); // Prevent garbage collection
	}
	return Instance;
}

void UOpenfortOpenfortSDK::Initialize(const FString &PublishableKey, const FString &ShieldPublishableKey, const FString &ShieldEncryptionKey, bool ShieldDebug, const FString &BackendUrl, const FString &IframeUrl, const FString &ShieldUrl, const FString &Deeplink, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate)
{
	check(JSConnector.IsValid());

	FOpenfortOpenfortSDKInitData InitData;
	InitData.publishableKey = PublishableKey;
	InitData.shieldPublishableKey = ShieldPublishableKey;
	InitData.shieldEncryptionKey = ShieldEncryptionKey;
	InitData.shieldDebug = ShieldDebug;
	InitData.backendUrl = BackendUrl;
	InitData.iframeUrl = IframeUrl;
	InitData.shieldUrl = ShieldUrl;

	this->Deeplink = Deeplink;

	CallJS(OpenfortOpenfortSDKAction::INIT, InitData.ToJsonString(), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitializeResponse), false);
}

void UOpenfortOpenfortSDK::SetCallTimeout(int32 Milliseconds)
{
	// Implement this method to set the timeout for JS calls
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
	CallJS(OpenfortOpenfortSDKAction::INIT_OAUTH, UStructToJsonString(Request), ResponseDelegate, FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnInitOAuthResponse));
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

void UOpenfortOpenfortSDK::ClearCache(bool IncludeDiskFiles)
{
	// Implement cache clearing
}

void UOpenfortOpenfortSDK::ClearStorage()
{
	// Implement storage clearing
}

void UOpenfortOpenfortSDK::OnDeepLinkActivated(const FString &Url)
{
	// Implement deep link handling
}

void UOpenfortOpenfortSDK::BroadcastAuthEvent(EOpenfortAuthEvent AuthEvent)
{
	OnAuthEvent.Broadcast(AuthEvent);
}

UOpenfortOpenfortSDK::UOpenfortOpenfortSDK()
{
	bIsInitialized = false;
	bIsLoggedIn = false;
}

void UOpenfortOpenfortSDK::BeginDestroy