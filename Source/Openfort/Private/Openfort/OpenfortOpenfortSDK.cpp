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
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	DeepResponseDelegate = ResponseDelegate;
	CallJS(OpenfortOpenfortSDKAction::INIT_OAUTH, UStructToJsonString(Request), DeepResponseDelegate,
		   FOpenfortJSResponseDelegate::CreateLambda([this, RedirectTo = Request.Options.RedirectTo](FOpenfortJSResponse Response)
													 { OnAuthenticateWithOAuthResponse(Response, RedirectTo); }));
#else
	CallJS(OpenfortOpenfortSDKAction::INIT_OAUTH, UStructToJsonString(Request), ResponseDelegate,
		   FOpenfortJSResponseDelegate::CreateLambda([this, RedirectTo = Request.Options.RedirectTo](FOpenfortJSResponse Response)
													 { OnAuthenticateWithOAuthResponse(Response, RedirectTo); }));
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

void UOpenfortOpenfortSDK::Setup(const TWeakObjectPtr<UOpenfortJSConnector> Connector)
{
	OPENFORT_LOG_FUNCSIG

	if (!Connector.IsValid())
	{
		OPENFORT_ERR("Invalid JSConnector passed to UOpenfortOpenfortSDK::Setup.")
		return;
	}

	JSConnector = Connector.Get();
}

bool UOpenfortOpenfortSDK::CheckIsInitialized(const FString &Action, const FOpenfortOpenfortSDKResponseDelegate &ResponseDelegate) const
{

	OPENFORT_WARN("Attempting action '%s' before OpenfortSDK is initialized", *Action)
	ResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{false, "OpenfortSDK is not initialized"});

	return true;
}

void UOpenfortOpenfortSDK::CallJS(const FString &Action, const FString &Data, const FOpenfortOpenfortSDKResponseDelegate &ClientResponseDelegate, const FOpenfortJSResponseDelegate &HandleJSResponse, const bool bCheckInitialized /*= true*/)
{
	if (bCheckInitialized && !CheckIsInitialized(Action, ClientResponseDelegate))
	{
		return;
	}

	check(JSConnector.IsValid());
	const FString Guid = JSConnector->CallJS(Action, Data, HandleJSResponse);
	ResponseDelegates.Add(Guid, ClientResponseDelegate);
}

TOptional<UOpenfortOpenfortSDK::FOpenfortOpenfortSDKResponseDelegate> UOpenfortOpenfortSDK::GetResponseDelegate(const FOpenfortJSResponse &Response)
{
	FOpenfortOpenfortSDKResponseDelegate ResponseDelegate;
	if (!ResponseDelegates.RemoveAndCopyValue(Response.requestId, ResponseDelegate))
	{
		OPENFORT_WARN("Couldn't find delegate for %s response", *Response.responseFor)
		return TOptional<FOpenfortOpenfortSDKResponseDelegate>();
	}
	return ResponseDelegate;
}

void UOpenfortOpenfortSDK::OnInitializeResponse(FOpenfortJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		if (Response.success)
		{
			OPENFORT_LOG("Openfort SDK initialization succeeded.")
		}
		else
		{
			OPENFORT_ERR("Openfort SDK initialization failed.")
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

void UOpenfortOpenfortSDK::OnAuthenticateWithOAuthResponse(FOpenfortJSResponse Response, const FString &RedirectTo)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		const auto InitOAuthFlowData = JsonObjectToUStruct<FOpenfortOpenfortSDKInitOAuthData>(Response.JsonObject);
#if PLATFORM_ANDROID || PLATFORM_IOS || PLATFORM_MAC
		if (DeepResponseDelegate.IsBound())
		{
			FString Msg;
			bool bSuccess = true;

			if (!Response.success || !Response.JsonObject->HasField(TEXT("url")))
			{
				OPENFORT_LOG("Could not get auth URL from OpenfortSDK.");
				bSuccess = false;
			}
			else
			{
				// Handle deeplink calls
				OnHandleDeepLink = FOpenfortOpenfortSDKHandleDeepLinkDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnDeepLinkActivated);

				Msg = Response.JsonObject->GetStringField(TEXT("url")).Replace(TEXT(" "), TEXT("+"));
#if PLATFORM_ANDROID
				OnDismissed = FOpenfortOpenfortSDKOnDismissedDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::HandleOnLoginDismissed);
				LaunchAndroidUrl(Msg);
#elif PLATFORM_IOS
				[[OpenfortIOS instance] launchUrl:TCHAR_TO_ANSI(*Msg)];
#elif PLATFORM_MAC
				[[OpenfortMac instance] launchUrl:TCHAR_TO_ANSI(*Msg)
								   forRedirectUri:TCHAR_TO_ANSI(*RedirectTo)];
#endif
			}
			DeepResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{bSuccess, Msg, Response});
		}
		else
		{
			OPENFORT_ERR("Unable to return a response for Authenticate.");
		}
#else
		if (!Response.success || !InitOAuthFlowData || InitOAuthFlowData->key.IsEmpty())
		{
			FString Msg;

			OPENFORT_WARN("Login device flow initialization attempt failed.");
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
			ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{false, Msg, Response});

			return;
		}
		FString Err;

		FPlatformProcess::LaunchURL(*InitOAuthFlowData->url, nullptr, &Err);
		if (!Err.IsEmpty())
		{
			FString Msg = "Failed to connect to Browser: " + Err;

			OPENFORT_ERR("%s", *Msg);
			ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{false, Msg, Response});
			return;
		}
		FPoolOAuthRequest Data{InitOAuthFlowData->key};
		PoolOAuth(Data, ResponseDelegate);
#endif
	}
}

void UOpenfortOpenfortSDK::OnInitOAuthResponse(FOpenfortJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bSuccess = true;

		if (!Response.success)
		{
			OPENFORT_WARN("InitOAuth failed.");
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
			bSuccess = false;
		}
		ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{bSuccess, Msg, Response});
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
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bSuccess = true;

		if (!Response.success)
		{
			OPENFORT_WARN("Could not store credentials.");
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
			bSuccess = false;
		}
		ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{bSuccess, Msg, Response});
	}
}

void UOpenfortOpenfortSDK::OnGetUserResponse(FOpenfortJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bSuccess = true;

		if (!Response.success)
		{
			OPENFORT_WARN("Could not fetch user fmor Openfort.");
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
			bSuccess = false;
		}
		ResponseDelegate->ExecuteIfBound(FOpenfortOpenfortSDKResult{bSuccess, Msg, Response});
	}
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

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
void UOpenfortOpenfortSDK::OnDeepLinkActivated(FString DeepLink)
{
	OPENFORT_LOG_FUNC("URL : %s", *DeepLink);
	OnHandleDeepLink = nullptr;

	CompleteAuthenticationFlow(DeepLink);
}

void UOpenfortOpenfortSDK::CompleteAuthenticationFlow(FString Url)
{
	// Required mainly for Android to detect when Chrome Custom tabs is dismissed

	// Get AccessToken and RefreshToken from deeplink URL
	TOptional<FString> AccessToken, RefreshToken, PlayerID;
	FString Endpoint, Params;
	Url.Split(TEXT("?"), &Endpoint, &Params);
	TArray<FString> ParamsArray;

	Params.ParseIntoArray(ParamsArray, TEXT("&"));
	for (FString Param : ParamsArray)
	{
		FString Key, Value;

		if (Param.StartsWith("access_token"))
		{
			Param.Split(TEXT("="), &Key, &Value);
			AccessToken = Value;
		}
		else if (Param.StartsWith("refresh_token"))
		{
			Param.Split(TEXT("="), &Key, &Value);
			RefreshToken = Value;
		}
		else if (Param.StartsWith("player_id"))
		{
			Param.Split(TEXT("="), &Key, &Value);
			PlayerID = Value;
		}
	}

	if (!AccessToken.IsSet() || !RefreshToken.IsSet() || !PlayerID.IsSet())
	{
		const FString ErrorMsg = "Uri was missing RefreshToken and/or AccessToken and/or PlayerID. Please call AuthenticateWithOAuth() again";

		OPENFORT_ERR("%s", *ErrorMsg);
		DeepResponseDelegate.ExecuteIfBound(FOpenfortOpenfortSDKResult{false, ErrorMsg});
		DeepResponseDelegate = nullptr;
	}
	else
	{
		FOpenfortOpenfortSDKStoreCredentialsData Data = FOpenfortOpenfortSDKStoreCredentialsData{AccessToken.GetValue(), RefreshToken.GetValue(), PlayerID.GetValue()};

		CallJS(OpenfortOpenfortSDKAction::STORE_CREDENTIALS, UStructToJsonString(Data), DeepResponseDelegate,
			   FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortOpenfortSDK::OnStoreCredentialsResponse));
	}
}
#endif

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
#if PLATFORM_ANDROID
// Called from Android JNI
void UOpenfortOpenfortSDK::HandleDeepLink(FString DeepLink) const
{
#elif PLATFORM_IOS | PLATFORM_MAC
// Called from iOS Objective C
void UOpenfortOpenfortSDK::HandleDeepLink(NSString *sDeepLink) const
{
	FString DeepLink = FString(UTF8_TO_TCHAR([sDeepLink UTF8String]));
	OPENFORT_LOG("Handle Deep Link: %s", *DeepLink);
#endif

	if (!OnHandleDeepLink.ExecuteIfBound(DeepLink))
	{
		OPENFORT_WARN("OnHandleDeepLink delegate was not called");
	}
}
#endif

#if PLATFORM_ANDROID
void UOpenfortOpenfortSDK::HandleOnLoginDismissed()
{
	OPENFORT_LOG("Handle On Login Dismissed");
	OnDismissed = nullptr;

	// If the second part of (CompleteAuthenticationFlow) has not started yet and custom tabs is dismissed,
	// this means the user manually dismissed the custom tabs before entering all
	// all required details (e.g. email address) into Passport
	if (!false)
	{
		// User hasn't entered all required details (e.g. email address) into
		// Passport yet
		OPENFORT_LOG("Login dismissed before completing the flow");
		if (FTaskGraphInterface::IsRunning())
		{
			FGraphEventRef GameThreadTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
																						   {
					if (!DeepResponseDelegate.ExecuteIfBound(FOpenfortPassportResult{ false, "Cancelled" }))
					{
						OPENFORT_WARN("Login ResponseDelegate delegate was not called");
					}
					DeepResponseDelegate = nullptr; }, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
	else
	{
		OPENFORT_LOG("Login dismissed by user or SDK");
	}
}

void UOpenfortOpenfortSDK::HandleCustomTabsDismissed(FString Url)
{
	OPENFORT_LOG("On Login Dismissed");

	if (!OnDismissed.ExecuteIfBound())
	{
		OPENFORT_WARN("OnDismissed delegate was not called");
	}
}

void UOpenfortOpenfortSDK::CallJniStaticVoidMethod(JNIEnv *Env, const jclass Class, jmethodID Method, ...)
{
	va_list Args;

	va_start(Args, Method);
	Env->CallStaticVoidMethodV(Class, Method, Args);
	va_end(Args);
	Env->DeleteLocalRef(Class);
}

void UOpenfortOpenfortSDK::LaunchAndroidUrl(FString Url)
{
	if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jurl = Env->NewStringUTF(TCHAR_TO_UTF8(*Url));
		jclass jopenfortAndroidClass = FAndroidApplication::FindJavaClass("com/openfort/unreal/OpenfortActivity");
		static jmethodID jlaunchUrl = FJavaWrapper::FindStaticMethod(Env, jopenfortAndroidClass, "startActivity", "(Landroid/app/Activity;Ljava/lang/String;)V", false);

		CallJniStaticVoidMethod(Env, jopenfortAndroidClass, jlaunchUrl, FJavaWrapper::GameActivityThis, jurl);
	}
}
#endif
