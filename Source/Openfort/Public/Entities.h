#pragma once

#include "Entities.generated.h"

USTRUCT()
struct FErrorResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString message;
	
	FErrorResponse() {}
};

USTRUCT(BlueprintType, Category = "SignatureSessionResponse")
struct OPENFORT_API FSignatureSessionResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FErrorResponse error;
	
	UPROPERTY(BlueprintReadOnly)
	FString id;
	
	UPROPERTY(BlueprintReadOnly)
	FString created_at;
	
	UPROPERTY(BlueprintReadOnly)
	FString valid_until;
};

USTRUCT(BlueprintType, Category = "SignatureSessionResponse")
struct OPENFORT_API FTransactionIntentResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FErrorResponse error;
	
	UPROPERTY(BlueprintReadOnly)
	FString id;
	
	UPROPERTY(BlueprintReadOnly)
	FString created_at;
	
	UPROPERTY(BlueprintReadOnly)
	FString updated_at;
	
	UPROPERTY(BlueprintReadOnly)
	int chain_id;
	
	UPROPERTY(BlueprintReadOnly)
	FString user_operation_hash;
	
	UPROPERTY(BlueprintReadOnly)
	FString policy;
	
	UPROPERTY(BlueprintReadOnly)
	FString player;
	
	UPROPERTY(BlueprintReadOnly)
	FString account;

};
