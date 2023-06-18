// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyPair.h"
#include "ApiClient.h"
#include "Entities.h"
#include "OpenfortComponent.generated.h"

class UOpenfortComponent;
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FSignatureSessionComplete, UOpenfortComponent, OnSignatureSessionComplete, FSignatureSessionResponse, Response);

class UOpenfortComponent;
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FSignatureTransactionIntentComplete, UOpenfortComponent, OnSignatureSessionComplete, FTransactionIntentResponse, Response);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENFORT_API UOpenfortComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenfortComponent();

private:
	class FString API_KEY;

	ApiClient apiClient;

	ApiClient GetApiClient();

	KeyPair keyPair;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SetApiKey(const FString& apiKey);		

	/** Generates a new key pair */
	UFUNCTION(BlueprintCallable, Category = "Session")
	bool GenerateSessionKey();		

	/** Generates a new key pair */
	UFUNCTION(BlueprintCallable, Category = "Session")
	bool InitializeSessionKey(const FString& privateKey);

	UFUNCTION(BlueprintCallable, Category = "Session")
	FString GetSessionPublicKeyHex();

	UFUNCTION(BlueprintCallable, Category = "Session")
	FString GetSessionPrivateKeyHex();

	UFUNCTION(BlueprintCallable, Category = "Session")
	FText SignMessage(const FString& message);		

	/** Sends a request to session signature endpoint */
	UFUNCTION(BlueprintCallable, Category = "Api")
	void SignatureSession(const FString& session, const FString& signature); 

	/** Sends a request to session signature endpoint */
	UFUNCTION(BlueprintCallable, Category = "Api")
	void SignatureTransactionIntent(const FString& transactionIntentId, const FString& signature); 
	
	UPROPERTY(BlueprintAssignable)
    FSignatureSessionComplete OnSignatureSessionComplete;
	
	UPROPERTY(BlueprintAssignable)
    FSignatureTransactionIntentComplete OnSignatureTransactionIntentComplete;
};
