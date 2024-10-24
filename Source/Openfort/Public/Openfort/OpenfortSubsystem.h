// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/EngineBaseTypes.h"
#include "Runtime/Launch/Resources/Version.h"
// clang-format off
#include "OpenfortSubsystem.generated.h"
// clang-format on

class UOpenfortJSConnector;
class UOpenfortOpenfortSDK;

DECLARE_MULTICAST_DELEGATE_OneParam(FOpenfortSubsystemReadyDelegate, TWeakObjectPtr<UOpenfortJSConnector>);

/**
 *
 */
UCLASS()
class OPENFORT_API UOpenfortSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UOpenfortSubsystem();

	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;

	TWeakObjectPtr<UOpenfortOpenfortSDK> GetOpenfortSDK() const;

	bool IsReady() const { return bIsReady; }

	// FOnGameViewportTick& OnGameViewportTick() { return GameViewportTickEvent; }

	// Execute a delegate when the subsystem is ready (i.e.: when the browser is
	// running and the Openfort SDK game bridge has loaded).
	template <class UserClass>
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	void WhenReady(UserClass *Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TMethodPtr<UserClass> Func);
#else
	void WhenReady(UserClass *Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func);
#endif

private:
	UPROPERTY()
	class UOpenfortBrowserUserWidget *BrowserWidget = nullptr;

	UPROPERTY()
	UOpenfortOpenfortSDK *OpenfortSDK = nullptr;

	bool bHasSetupGameBridge = false;
	bool bIsReady = false;
	FOpenfortSubsystemReadyDelegate OnReady;

	FDelegateHandle WorldTickHandle;
	FDelegateHandle ViewportCreatedHandle;
#if PLATFORM_ANDROID | PLATFORM_IOS
	FDelegateHandle EngineInitCompleteHandle;
#endif

	void SetupGameBridge();
	void OnBridgeReady();
	void ManageBridgeDelegateQueue();
	void OnViewportCreated();
	void WorldTickStart(UWorld *World, ELevelTick TickType, float DeltaSeconds);
};
