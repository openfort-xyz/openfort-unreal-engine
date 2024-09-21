// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Ensure you include the header for UOpenfortOpenfortSDK
#include "OpenfortOpenfortSDK.h"  // Adjust this path as necessary

// clang-format off
#include "OpenfortSubsystem.generated.h"
// clang-format on

DECLARE_MULTICAST_DELEGATE_OneParam(FOpenfortSubsystemReadyDelegate, TWeakObjectPtr<class UOpenfortJSConnector>);

/**
 * Openfort Subsystem class that handles the integration with the Openfort SDK.
 */
UCLASS()
class OPENFORT_API UOpenfortSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UOpenfortSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Get the Openfort SDK instance. */
	UOpenfortOpenfortSDK* GetOpenfortSDK() const { return OpenfortSDK; }

	/** Check if the subsystem is ready for use. */
	bool IsReady() const { return bIsReady; }

	/**
	 * Bind a function to be called when the subsystem is ready.
	 *
	 * @param Object The object to bind the function to.
	 * @param Func The function to bind.
	 */
	template <class UserClass>
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	void WhenReady(UserClass* Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TMethodPtr<UserClass> Func);
#else
	void WhenReady(UserClass* Object, typename FOpenfortSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func);
#endif

private:
	UPROPERTY()
	UOpenfortOpenfortSDK* OpenfortSDK = nullptr;

	UPROPERTY()
	class UOpenfortBrowserUserWidget* BrowserWidget = nullptr;

	UPROPERTY()
	class UOpenfortBlui* OpenfortBlui = nullptr;

	bool bHasSetupGameBridge = false;
	bool bIsReady = false;
	FOpenfortSubsystemReadyDelegate OnReady;

	FDelegateHandle WorldTickHandle;
	FDelegateHandle ViewportCreatedHandle;

#if PLATFORM_ANDROID || PLATFORM_IOS
	FDelegateHandle EngineInitCompleteHandle;
#endif

	void SetupGameBridge();
	void OnBridgeReady();
	void ManageBridgeDelegateQueue();
	void OnViewportCreated();
	void WorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds);
};

// Explicit instantiation declarations for WhenReady, if needed
// template void UOpenfortSubsystem::WhenReady<UOpenfortOpenfortSDKAuthenticateAsyncActions>(UOpenfortOpenfortSDKAuthenticateAsyncActions *, typename FOpenfortSubsystemReadyDelegate::FDelegate::TMethodPtr<UOpenfortOpenfortSDKAuthenticateAsyncActions>);
// Add more explicit instantiation declarations as necessary...
