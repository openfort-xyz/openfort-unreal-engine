// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/OpenfortComponent.h"

// Sets default values for this component's properties
UOpenfortComponent::UOpenfortComponent() //: apiClient(new ApiClient(API_KEY, ApiClient::DEFAULT_BASE_PATH))
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenfortComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOpenfortComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UOpenfortComponent::GenerateSessionKey()
{
	return keyPair.Generate();
}

bool UOpenfortComponent::InitializeSessionKey(const FString& secKeyHex)
{
	return keyPair.Initialize(std::string(TCHAR_TO_UTF8(*secKeyHex)));
}

FString UOpenfortComponent::GetSessionPublicKeyHex()
{
	return keyPair.GetPublicKeyHex().c_str();
}

FString UOpenfortComponent::GetSessionPrivateKeyHex()
{
	return keyPair.GetPrivateKeyHex().c_str();
}

FString UOpenfortComponent::GetSessionKeyAddress()
{
	return keyPair.GetAddress().c_str();
}

void UOpenfortComponent::SetApiKey(const FString& apiKey)
{
	API_KEY = apiKey;
}

FText UOpenfortComponent::SignMessage(const FString& message)
{
	return FText::FromString(keyPair.Sign(std::string(TCHAR_TO_UTF8(*message))).c_str());
}

void UOpenfortComponent::SignatureSession(const FString& session, const FString& signature)
{
	ApiClient client = ApiClient(API_KEY);
	client.SignatureSession(session, signature, OnSignatureSessionComplete);
}

void UOpenfortComponent::SignatureTransactionIntent(const FString& transactionIntentId, const FString& signature)
{
	ApiClient client = ApiClient(API_KEY);
	client.SignatureTransactionIntent(transactionIntentId, signature, OnSignatureTransactionIntentComplete);
}
