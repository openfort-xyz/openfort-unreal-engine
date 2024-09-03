// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/OpenfortJSConnector.h"

#include "Openfort/Misc/OpenfortLogging.h"
#include "OpenfortBrowserWidget.h"
#include "Openfort/OpenfortJSMessages.h"

UOpenfortJSConnector::UOpenfortJSConnector() { OPENFORT_LOG_FUNCSIG }

void UOpenfortJSConnector::Init(bool bPageLoaded)
{
	OPENFORT_LOG("JSConnect::Init called, bPageloaded %d", bPageLoaded);
	if (bPageLoaded)
	{
		OPENFORT_LOG("Browser finished loading the bridge document before the "
					 "Openfort JSConnector was created & bound")
		FOpenfortJSResponse InitResponse{};
		InitResponse.responseFor = "init";
		InitResponse.requestId = "1";
		InitResponse.success = true;
		HandleInitResponse(InitResponse);
	}
	else
	{
		// Add init function to "waiting for response" queue to handle the init
		// message
		RequestResponseDelegates.Add("1", FOpenfortJSResponseDelegate::CreateUObject(this, &UOpenfortJSConnector::HandleInitResponse));
	}
}

void UOpenfortJSConnector::PostInitProperties() { UObject::PostInitProperties(); }

bool UOpenfortJSConnector::IsBound() const { return bIsBound; }

bool UOpenfortJSConnector::IsBridgeReady() const { return bIsBridgeReady; }

void UOpenfortJSConnector::AddCallbackWhenBridgeReady(const FOnBridgeReadyDelegate::FDelegate &Delegate)
{
	if (IsBridgeReady())
	{
		Delegate.Execute();
	}
	else
	{
		OnBridgeReady.Add(Delegate);
	}
}

FString UOpenfortJSConnector::CallJS(const FString &Function, const FString &Data, const FOpenfortJSResponseDelegate &HandleResponse, const float ResponseTimeout)
{
	const FString Guid = FGuid::NewGuid().ToString();
	CallJS(FOpenfortJSRequest{Function, Data, Guid}, HandleResponse, ResponseTimeout);
	return Guid;
}

void UOpenfortJSConnector::CallJS(const FOpenfortJSRequest &Request, FOpenfortJSResponseDelegate HandleResponse, float ResponseTimeout)
{
	if (!IsBridgeReady())
	{
		// OPENFORT_WARN("Attempt to call Openfort JS bridge before bridge ready")
		AddCallbackWhenBridgeReady(FOnBridgeReadyDelegate::FDelegate::CreateLambda([Request, HandleResponse, ResponseTimeout, this]()
																				   { CallJS(Request, HandleResponse, ResponseTimeout); }));
		return;
	}

	// First, add response callback to map
	RequestResponseDelegates.Add(Request.requestId, HandleResponse);

	// Convert the request to a string
	FString RequestString = Request.ToJsonString();
	RequestString.ReplaceInline(TEXT("\\"), TEXT("\\\\"));
	RequestString.ReplaceInline(TEXT("\""), TEXT("\\\""));
	const FString JS = FString::Printf(TEXT("callFunction(\"%s\");"), *RequestString);
	// const FString JS = FString::Printf(TEXT("callFunction(%s);"),
	// *RequestString);

	// Execute
	OPENFORT_LOG("Executing JS: %s", *JS)
	const bool Result = ExecuteJs.ExecuteIfBound(JS);
	if (!Result)
	{
		OPENFORT_WARN("ExecuteJS delegate was not called");
	}

	// TODO: add timeout callback
}

void UOpenfortJSConnector::HandleInitResponse(FOpenfortJSResponse Response)
{
	OPENFORT_LOG_FUNCSIG
	bIsBridgeReady = true;
	OnBridgeReady.Broadcast();
	OnBridgeReady.Clear();
}

void UOpenfortJSConnector::SendToGame(FString Message)
{
	OPENFORT_LOG_FUNC("Received message from JS: %s", *Message);

	// Parse response

	const TOptional<FOpenfortJSResponse> Response = FOpenfortJSResponse::FromJsonString(Message);
	if (!Response.IsSet())
	{
		OPENFORT_WARN("Received unexpected response from browser: %s", *Message);
		return;
	}

	// Handle response

	if (!RequestResponseDelegates.Contains(Response->requestId))
	{
		OPENFORT_WARN("No delegate found for response with id %s", *Response->requestId);
		return;
	}

	if (!RequestResponseDelegates[Response->requestId].ExecuteIfBound(Response.GetValue()))
	{
		OPENFORT_WARN("Delegate for response with id %s failed to execute", *Response->requestId);
	}

	RequestResponseDelegates.Remove(Response->requestId);
}

#if PLATFORM_ANDROID | PLATFORM_IOS
void UOpenfortJSConnector::SetMobileBridgeReady()
{
	OPENFORT_LOG_FUNCSIG
	bIsBridgeReady = true;
	OnBridgeReady.Broadcast();
	OnBridgeReady.Clear();
}
#endif
