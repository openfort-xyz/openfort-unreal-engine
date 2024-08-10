// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "OpenfortBrowserUserWidget.generated.h"

UCLASS()
class OPENFORT_API UOpenfortBrowserUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void BeginDestroy() override;
	virtual void RemoveFromParent() override;
	virtual void OnWidgetRebuilt() override;

	TWeakObjectPtr<class UOpenfortJSConnector> GetJSConnector() const;

private:
	UPROPERTY()
	class UOpenfortBrowserWidget *Browser = nullptr;

	bool bIsBrowserAppInitialized = false;

	FTimerHandle Timer;

	FMargin DefaultOffsets = FMargin(150, 150, 150, 150);
};
