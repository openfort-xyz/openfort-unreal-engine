#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OpenfortRequests.generated.h"

USTRUCT(BlueprintType)
struct OPENFORT_API FOpenfortExecuteTransactionRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	int32 ChainId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString ContractId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString PolicyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString FunctionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	TArray<FString> FunctionArgs;

	FOpenfortExecuteTransactionRequest()
		: ChainId(0)
	{
		FunctionArgs = TArray<FString>();
	}

	FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct OPENFORT_API FAuthCredentialsRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Player;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AccessToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString RefreshToken;

	FAuthCredentialsRequest() {}
	FAuthCredentialsRequest(const FString &InPlayer, const FString &InAccessToken, const FString &InRefreshToken)
		: Player(InPlayer), AccessToken(InAccessToken), RefreshToken(InRefreshToken) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FAuthenticateWithSiweRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Signature;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Message;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString WalletClientType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString ConnectorType;

	FAuthenticateWithSiweRequest() {}
	FAuthenticateWithSiweRequest(const FString &InSignature, const FString &InMessage, const FString &InWalletClientType, const FString &InConnectorType)
		: Signature(InSignature), Message(InMessage), WalletClientType(InWalletClientType), ConnectorType(InConnectorType) {}
};

UENUM(BlueprintType)
enum class EShieldAuthType : uint8
{
	Openfort UMETA(DisplayName = "Openfort"),
	Custom UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct OPENFORT_API FShieldAuthentication
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	EShieldAuthType Auth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Token;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthProvider;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString TokenType;

	FShieldAuthentication() : Auth(EShieldAuthType::Openfort) {}
	FShieldAuthentication(EShieldAuthType InAuth, const FString &InToken, const FString &InAuthProvider = TEXT(""), const FString &InTokenType = TEXT(""))
		: Auth(InAuth), Token(InToken), AuthProvider(InAuthProvider), TokenType(InTokenType) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FEmbeddedSignerRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	int32 ChainId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FShieldAuthentication ShieldAuthentication;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString RecoveryPassword;

	FEmbeddedSignerRequest() : ChainId(0) {}
	FEmbeddedSignerRequest(int32 InChainId, const FShieldAuthentication &InShieldAuthentication, const FString &InRecoveryPassword = TEXT(""))
		: ChainId(InChainId), ShieldAuthentication(InShieldAuthentication), RecoveryPassword(InRecoveryPassword) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FEthereumProviderOptions
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bAnnounceProvider;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Policy;

	FEthereumProviderOptions() : bAnnounceProvider(true) {}
	FEthereumProviderOptions(bool InAnnounceProvider, const FString &InPolicy = TEXT(""))
		: bAnnounceProvider(InAnnounceProvider), Policy(InPolicy) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FEthereumProviderRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FEthereumProviderOptions Options;

	FEthereumProviderRequest() {}
	FEthereumProviderRequest(const FEthereumProviderOptions &InOptions)
		: Options(InOptions) {}
};

UENUM(BlueprintType)
enum class EOAuthProvider : uint8
{
	discord UMETA(DisplayName = "discord"),
	epic_games UMETA(DisplayName = "epic_games"),
	facebook UMETA(DisplayName = "facebook"),
	google UMETA(DisplayName = "google"),
	twitter UMETA(DisplayName = "twitter")
};

USTRUCT(BlueprintType)
struct OPENFORT_API FOAuthInitRequestOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	TMap<FString, FString> QueryParams;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString RedirectTo;

	FOAuthInitRequestOptions() {}

	FOAuthInitRequestOptions(const TMap<FString, FString> &InQueryParams, const FString &InRedirectTo)
		: QueryParams(InQueryParams), RedirectTo(InRedirectTo) {}

	bool operator==(const FOAuthInitRequestOptions &Other) const
	{
		if (QueryParams.Num() != Other.QueryParams.Num() || RedirectTo != Other.RedirectTo)
		{
			return false;
		}

		for (const auto &Pair : QueryParams)
		{
			const FString *OtherValue = Other.QueryParams.Find(Pair.Key);
			if (!OtherValue || *OtherValue != Pair.Value)
			{
				return false;
			}
		}

		return true;
	}

	bool operator!=(const FOAuthInitRequestOptions &Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FOAuthInitRequestOptions &Options)
	{
		uint32 Hash = GetTypeHash(Options.RedirectTo);

		for (const auto &Pair : Options.QueryParams)
		{
			Hash = HashCombine(Hash, GetTypeHash(Pair.Key));
			Hash = HashCombine(Hash, GetTypeHash(Pair.Value));
		}

		return Hash;
	}

	FString ToString() const
	{
		FString Result = TEXT("FOAuthInitRequestOptions {\n");
		Result += TEXT("  QueryParams: {\n");
		for (const auto &Pair : QueryParams)
		{
			Result += FString::Printf(TEXT("    %s: %s\n"), *Pair.Key, *Pair.Value);
		}
		Result += TEXT("  }\n");
		Result += TEXT("  RedirectTo: ") + RedirectTo + TEXT("\n");
		Result += TEXT("}\n");
		return Result;
	}

	FString ToJson() const
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		TSharedPtr<FJsonObject> QueryParamsObject = MakeShareable(new FJsonObject);
		for (const auto &Pair : QueryParams)
		{
			QueryParamsObject->SetStringField(Pair.Key, Pair.Value);
		}
		JsonObject->SetObjectField(TEXT("QueryParams"), QueryParamsObject);

		JsonObject->SetStringField(TEXT("RedirectTo"), RedirectTo);

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

		return OutputString;
	}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FInitLinkOAuthRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	EOAuthProvider Provider;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FOAuthInitRequestOptions Options;

	FInitLinkOAuthRequest() {}
	FInitLinkOAuthRequest(EOAuthProvider InProvider, const FString &InAuthToken, const FOAuthInitRequestOptions &InOptions)
		: Provider(InProvider), AuthToken(InAuthToken), Options(InOptions) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FInitSiweRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Address;

	FInitSiweRequest() {}
	FInitSiweRequest(const FString &InAddress) : Address(InAddress) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FLinkEmailPasswordRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Password;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthToken;

	FLinkEmailPasswordRequest() {}
	FLinkEmailPasswordRequest(const FString &InEmail, const FString &InPassword, const FString &InAuthToken)
		: Email(InEmail), Password(InPassword), AuthToken(InAuthToken) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FLinkWalletRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Signature;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Message;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString WalletClientType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString ConnectorType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthToken;

	FLinkWalletRequest() {}
	FLinkWalletRequest(const FString &InSignature, const FString &InMessage, const FString &InWalletClientType, const FString &InConnectorType, const FString &InAuthToken)
		: Signature(InSignature), Message(InMessage), WalletClientType(InWalletClientType), ConnectorType(InConnectorType), AuthToken(InAuthToken) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FLoginEmailPasswordRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Password;

	FLoginEmailPasswordRequest() {}
	FLoginEmailPasswordRequest(const FString &InEmail, const FString &InPassword)
		: Email(InEmail), Password(InPassword) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FPoolOAuthRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Key;

	FPoolOAuthRequest() {}
	FPoolOAuthRequest(const FString &InKey) : Key(InKey) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FRegisterSessionRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString SessionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Signature;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bOptimistic;

	FRegisterSessionRequest() : bOptimistic(false) {}
	FRegisterSessionRequest(const FString &InSessionId, const FString &InSignature, bool InOptimistic = false)
		: SessionId(InSessionId), Signature(InSignature), bOptimistic(InOptimistic) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FRequestEmailVerificationRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString RedirectUrl;

	FRequestEmailVerificationRequest() {}
	FRequestEmailVerificationRequest(const FString &InEmail, const FString &InRedirectUrl)
		: Email(InEmail), RedirectUrl(InRedirectUrl) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignatureTransactionIntentRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString TransactionIntentId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString UserOperationHash;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Signature;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bOptimistic;

	FSignatureTransactionIntentRequest() : bOptimistic(false) {}
	FSignatureTransactionIntentRequest(const FString &InTransactionIntentId, const FString &InUserOperationHash = TEXT(""), const FString &InSignature = TEXT(""), bool InOptimistic = false)
		: TransactionIntentId(InTransactionIntentId), UserOperationHash(InUserOperationHash), Signature(InSignature), bOptimistic(InOptimistic) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignMessageOptions
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bHashMessage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bArrayifyMessage;

	FSignMessageOptions() : bHashMessage(false), bArrayifyMessage(false) {}
	FSignMessageOptions(bool InHashMessage, bool InArrayifyMessage)
		: bHashMessage(InHashMessage), bArrayifyMessage(InArrayifyMessage) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignMessageRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Message;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FSignMessageOptions Options;

	FSignMessageRequest() {}
	FSignMessageRequest(const FString &InMessage, const FSignMessageOptions &InOptions)
		: Message(InMessage), Options(InOptions) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FTypedDataField
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Type;

	FTypedDataField() {}
	FTypedDataField(const FString &InName, const FString &InType) : Name(InName), Type(InType) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FTypedDataDomain
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Version;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	int32 ChainId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString VerifyingContract;

	FTypedDataDomain() : ChainId(0) {}
	FTypedDataDomain(const FString &InName, const FString &InVersion, int32 InChainId, const FString &InVerifyingContract)
		: Name(InName), Version(InVersion), ChainId(InChainId), VerifyingContract(InVerifyingContract) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignTypedDataRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FTypedDataDomain Domain;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	TMap<FString, FString> Types;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	TMap<FString, FString> Value;

	FSignTypedDataRequest() {}
	FSignTypedDataRequest(const FTypedDataDomain &InDomain, const TMap<FString, FString> &InTypes, const TMap<FString, FString> &InValue)
		: Domain(InDomain), Types(InTypes), Value(InValue) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignupData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Name;

	FSignupData() {}
	FSignupData(const FString &InName) : Name(InName) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignupOptions
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FSignupData Data;

	FSignupOptions() {}
	FSignupOptions(const FSignupData &InData) : Data(InData) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FSignupEmailPasswordRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Password;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FSignupOptions Options;

	FSignupEmailPasswordRequest() {}
	FSignupEmailPasswordRequest(const FString &InEmail, const FString &InPassword, const FSignupOptions &InOptions)
		: Email(InEmail), Password(InPassword), Options(InOptions) {}
};

UENUM(BlueprintType)
enum class EThirdPartyOAuthProvider : uint8
{
	Accelbyte UMETA(DisplayName = "accelbyte"),
	Custom UMETA(DisplayName = "custom"),
	Firebase UMETA(DisplayName = "firebase"),
	Lootlocker UMETA(DisplayName = "lootlocker"),
	Oidc UMETA(DisplayName = "oidc"),
	Playfab UMETA(DisplayName = "playfab"),
	Supabase UMETA(DisplayName = "supabase")
};

UENUM(BlueprintType)
enum class ETokenType : uint8
{
	IdToken UMETA(DisplayName = "idToken"),
	CustomToken UMETA(DisplayName = "customToken")
};

USTRUCT(BlueprintType)
struct OPENFORT_API FThirdPartyProviderRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	EThirdPartyOAuthProvider Provider;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Token;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	ETokenType TokenType;

	FThirdPartyProviderRequest() {}
	FThirdPartyProviderRequest(EThirdPartyOAuthProvider InProvider, const FString &InToken, ETokenType InTokenType)
		: Provider(InProvider), Token(InToken), TokenType(InTokenType) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FUnlinkEmailPasswordRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthToken;

	FUnlinkEmailPasswordRequest() {}
	FUnlinkEmailPasswordRequest(const FString &InEmail, const FString &InAuthToken)
		: Email(InEmail), AuthToken(InAuthToken) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FUnlinkWalletRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Address;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthToken;

	FUnlinkWalletRequest() {}
	FUnlinkWalletRequest(const FString &InAddress, const FString &InAuthToken)
		: Address(InAddress), AuthToken(InAuthToken) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FValidateAndRefreshTokenRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bForceRefresh;

	FValidateAndRefreshTokenRequest() : bForceRefresh(false) {}
	explicit FValidateAndRefreshTokenRequest(bool InForceRefresh) : bForceRefresh(InForceRefresh) {}

	static FValidateAndRefreshTokenRequest Create(bool InForceRefresh = false)
	{
		return FValidateAndRefreshTokenRequest(InForceRefresh);
	}
};

UENUM(BlueprintType)
enum class ECodeChallengeMethod : uint8
{
	Plain UMETA(DisplayName = "Plain"),
	S256 UMETA(DisplayName = "S256")
};

USTRUCT(BlueprintType)
struct OPENFORT_API FCodeChallenge
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Challenge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	ECodeChallengeMethod Method;

	FCodeChallenge()
		: Method(ECodeChallengeMethod::Plain) {}

	FCodeChallenge(const FString &InChallenge, ECodeChallengeMethod InMethod = ECodeChallengeMethod::Plain)
		: Challenge(InChallenge), Method(InMethod) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FCodeChallengeVerify
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Verifier;

	FCodeChallengeVerify() {}

	explicit FCodeChallengeVerify(const FString &InVerifier)
		: Verifier(InVerifier) {}
};

// Update FVerifyEmailRequest to use FCodeChallengeVerify
USTRUCT(BlueprintType)
struct OPENFORT_API FVerifyEmailRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Token;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FCodeChallengeVerify Challenge;

	FVerifyEmailRequest() {}

	FVerifyEmailRequest(const FString &InEmail, const FString &InToken, const FCodeChallengeVerify &InChallenge = FCodeChallengeVerify())
		: Email(InEmail), Token(InToken), Challenge(InChallenge) {}
};

// Update FResetPasswordRequest to use FCodeChallenge
USTRUCT(BlueprintType)
struct OPENFORT_API FResetPasswordRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString Email;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString RedirectUrl;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FCodeChallenge Challenge;

	FResetPasswordRequest() {}

	FResetPasswordRequest(const FString &InEmail, const FString &InRedirectUrl, const FString &InState = TEXT(""), const FCodeChallenge &InChallenge = FCodeChallenge())
		: Email(InEmail), RedirectUrl(InRedirectUrl), State(InState), Challenge(InChallenge) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FUnlinkOAuthRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	EOAuthProvider Provider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FString AuthToken;

	FUnlinkOAuthRequest() {}
	FUnlinkOAuthRequest(EOAuthProvider InProvider, const FString &InAuthToken)
		: Provider(InProvider), AuthToken(InAuthToken) {}
};

USTRUCT(BlueprintType)
struct OPENFORT_API FOAuthInitRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	EOAuthProvider Provider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	FOAuthInitRequestOptions Options;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Openfort")
	bool bUsePooling;

	FOAuthInitRequest() : bUsePooling(false) {}
	FOAuthInitRequest(EOAuthProvider InProvider, const FOAuthInitRequestOptions &InOptions = FOAuthInitRequestOptions(), bool InUsePooling = false)
		: Provider(InProvider), Options(InOptions), bUsePooling(InUsePooling) {}
};

UENUM(BlueprintType)
enum class EOpenfortAuthEvent : uint8
{
	// Authorization
	LoggingIn UMETA(DisplayName = "Logging In"),
	LoginFailed UMETA(DisplayName = "Login Failed"),
	LoginSuccess UMETA(DisplayName = "Login Success"),
	LoggingOut UMETA(DisplayName = "Logging Out"),
	LogoutFailed UMETA(DisplayName = "Logout Failed"),
	LogoutSuccess UMETA(DisplayName = "Logout Success"),

	// Using saved credentials
	ReloggingIn UMETA(DisplayName = "Relogging In"),
	ReloginFailed UMETA(DisplayName = "Relogin Failed"),
	ReloginSuccess UMETA(DisplayName = "Relogin Success"),
	Reconnecting UMETA(DisplayName = "Reconnecting"),
	ReconnectFailed UMETA(DisplayName = "Reconnect Failed"),
	ReconnectSuccess UMETA(DisplayName = "Reconnect Success"),

	// Checking for saved credentials
	CheckingForSavedCredentials UMETA(DisplayName = "Checking For Saved Credentials"),
	CheckForSavedCredentialsFailed UMETA(DisplayName = "Check For Saved Credentials Failed"),
	CheckForSavedCredentialsSuccess UMETA(DisplayName = "Check For Saved Credentials Success")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthEventDelegate, EOpenfortAuthEvent, AuthEvent);