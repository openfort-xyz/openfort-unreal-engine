#pragma once

namespace OpenfortOpenfortSDKAction
{
	const FString INIT = TEXT("init");

	// EMAIL METHODS
	const FString LOGIN_WITH_EMAIL_PASSWORD = TEXT("logInWithEmailPassword");
	const FString SIGNUP_WITH_EMAIL_PASSWORD = TEXT("signUpWithEmailPassword");
	const FString LINK_EMAIL_PASSWORD = TEXT("linkEmailPassword");
	const FString UNLINK_EMAIL_PASSWORD = TEXT("unlinkEmailPassword");
	const FString REQUEST_RESET_PASSWORD = TEXT("requestResetPassword");
	const FString RESET_PASSWORD = TEXT("resetPassword");
	const FString REQUEST_EMAIL_VERIFICATION = TEXT("requestEmailVerification");
	const FString VERIFY_EMAIL = TEXT("verifyEmail");

	// OAUTH METHODS
	const FString INIT_OAUTH = TEXT("initOAuth");
	const FString INIT_LINK_OAUTH = TEXT("initLinkOAuth");
	const FString UNLINK_OAUTH = TEXT("unlinkOAuth");
	const FString POOL_OAUTH = TEXT("poolOAuth");

	// THIRD PARTY AUTH METHODS
	const FString AUTHENTICATE_WITH_THIRD_PARTY_PROVIDER = TEXT("authenticateWithThirdPartyProvider");

	// SIWE METHODS
	const FString INIT_SIWE = TEXT("initSIWE");
	const FString AUTHENTICATE_WITH_SIWE = TEXT("authenticateWithSIWE");
	const FString LINK_WALLET = TEXT("linkWallet");
	const FString UNLINK_WALLET = TEXT("unlinkWallet");

	// GENERAL AUTH METHODS
	const FString STORE_CREDENTIALS = TEXT("storeCredentials");
	const FString GET_ACCESS_TOKEN = TEXT("getAccessToken");
	const FString GET_USER = TEXT("getUser");
	const FString LOGOUT = TEXT("logout");
	const FString VALIDATE_AND_REFRESH_TOKEN = TEXT("validateAndRefreshToken");

	const FString SEND_SIGNATURE_TRANSACTION_INTENT_REQUEST = TEXT("sendSignatureTransactionIntentRequest");
	const FString SIGN_MESSAGE = TEXT("signMessage");
	const FString SIGN_TYPED_DATA = TEXT("signTypedData");
	const FString SEND_SIGNATURE_SESSION_REQUEST = TEXT("sendSignatureSessionRequest");
	const FString GET_EMBEDDED_STATE = TEXT("getEmbeddedState");
	const FString GET_ETHEREUM_PROVIDER = TEXT("getEthereumProvider");
	const FString CONFIGURE_SESSION_KEY = TEXT("configureSessionKey");
	const FString CONFIGURE_EMBEDDED_SIGNER = TEXT("configureEmbeddedSigner");
} // namespace OpenfortOpenfortSDKAction