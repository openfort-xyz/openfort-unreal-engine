// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/OpenfortDataTypes.h"

FString FOpenfortOpenfortSDKInitData::ToJsonString() const
{
	FString OutString;
	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

	if (!Wrapper.JsonObject.IsValid())
	{
		OPENFORT_ERR("Could not convert FOpenfortOpenfortSDKInitData to JSON")
		return "";
	}

	Wrapper.JsonObjectToString(OutString);

	return OutString;
}

TOptional<FOpenfortOpenfortSDKInitDeviceFlowData> FOpenfortOpenfortSDKInitDeviceFlowData::FromJsonString(const FString &JsonObjectString)
{
	FOpenfortOpenfortSDKInitDeviceFlowData OpenfortSDKConnect;

	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &OpenfortSDKConnect, 0, 0))
	{
		OPENFORT_WARN("Could not parse response from JavaScript into the expected "
					  "OpenfortSDK connect format")
		return TOptional<FOpenfortOpenfortSDKInitDeviceFlowData>();
	}

	return OpenfortSDKConnect;
}
