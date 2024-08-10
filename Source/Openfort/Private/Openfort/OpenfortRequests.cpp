#include "Openfort/OpenfortRequests.h"
#include "JsonObjectConverter.h"
#include "JsonObjectWrapper.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "Policies/CondensedJsonPrintPolicy.h"

FString FOpenfortExecuteTransactionRequest::ToJsonString() const
{
	FString OutString;
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	if (!FJsonObjectConverter::UStructToJsonObject(FOpenfortExecuteTransactionRequest::StaticStruct(), this, JsonObject, 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not convert FOpenfortExecuteTransactionRequest to JSON"));
		return "";
	}

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutString);
	if (!FJsonSerializer::Serialize(JsonObject, Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not serialize JSON object"));
		return "";
	}

	return OutString;
}