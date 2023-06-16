// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyPair.h"
#include "ApiClient.h"
#include "OpenfortComponent.generated.h"

struct SessionParams
{
public:
	std::string session;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENFORT_API UOpenfortComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenfortComponent();

private:
	ApiClient apiClient;

	ApiClient GetApiClient();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	KeyPair keyPair;

	/** Generates a new key pair */
	UFUNCTION(BlueprintCallable, Category = "Session")
	void GenerateKey();		

	UFUNCTION(BlueprintCallable, Category = "Session")
	FText SignMessage(const FString& message);		

	/** Sends a request to session signature endpoint */
	UFUNCTION(BlueprintCallable, Category = "Api")
	void SignatureSession(const FString& session, const FString& signature); 
};
