// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/OpenfortComponent.h"
#include "Misc/MessageDialog.h"


class FString API_KEY = TEXT("AAAA");

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

void UOpenfortComponent::GenerateKey()
{
	keyPair.Generate();
}

FText UOpenfortComponent::SignMessage(const FString& message)
{
	return FText::FromString(keyPair.Sign(std::string(TCHAR_TO_UTF8(*message))).c_str());
}

void UOpenfortComponent::SignatureSession(const FString& session, const FString& signature)
{
	ApiClient client = ApiClient(API_KEY);
	client.SignatureSession(session, signature);
}
