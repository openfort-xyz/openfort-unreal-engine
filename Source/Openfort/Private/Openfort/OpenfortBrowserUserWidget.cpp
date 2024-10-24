// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenfortBrowserUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/ScaleBox.h"
#include "Components/ScaleBoxSlot.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "OpenfortBrowserWidget.h"
#include "Openfort/OpenfortJSConnector.h"

TSharedRef<SWidget> UOpenfortBrowserUserWidget::RebuildWidget()
{
	// RebuildWidget is not called until the widget is first added to the
	// viewport.

	UPanelWidget *RootWidget = Cast<UPanelWidget>(GetRootWidget());

	if (!RootWidget)
	{
		RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));
		if (UCanvasPanelSlot *RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot))
		{
			// Format root widget
		}
		WidgetTree->RootWidget = RootWidget;
	}

	if (RootWidget)
	{
		UScaleBox *ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("ScaleBox"));
		RootWidget->AddChild(ScaleBox);
		if (ScaleBox)
		{
			Browser = WidgetTree->ConstructWidget<UOpenfortBrowserWidget>(UOpenfortBrowserWidget::StaticClass(), TEXT("OpenfortBrowserWidget"));
			ScaleBox->AddChild(Browser);
			if (UCanvasPanelSlot *RootWidgetSlot = Cast<UCanvasPanelSlot>(ScaleBox->Slot))
			{
#if PLATFORM_ANDROID | PLATFORM_IOS
				// Android webview needs to be at least 1px to 1px big to work
				// but it can be off screen
				RootWidgetSlot->SetAnchors(FAnchors(0, 0, 0, 0));
				RootWidgetSlot->SetOffsets(FMargin(-1, -1, 1, 1));
#else
				RootWidgetSlot->SetAnchors(FAnchors(0, 0, 1, 1));
				RootWidgetSlot->SetOffsets(DefaultOffsets);
#endif
			}
			if (UScaleBoxSlot *ScaleBoxSlot = Cast<UScaleBoxSlot>(Browser->Slot))
			{
				ScaleBoxSlot->SetHorizontalAlignment(HAlign_Fill);
				ScaleBoxSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}
	}

	return Super::RebuildWidget();
}

void UOpenfortBrowserUserWidget::BeginDestroy() { Super::BeginDestroy(); }

void UOpenfortBrowserUserWidget::RemoveFromParent()
{
#if !PLATFORM_ANDROID
	// This is all that is needed to persist the widget state outside throughout
	// level/world changes. Super::RemoveFromParent();

	if (UPanelWidget *CurrentParent = GetParent())
	{
		CurrentParent->RemoveChild(this);
	}
#endif
}

void UOpenfortBrowserUserWidget::OnWidgetRebuilt()
{
#if PLATFORM_ANDROID | PLATFORM_IOS
	// Mobile webview needs to be visible to work
#else
	SetVisibility(ESlateVisibility::Collapsed);
#endif
	Super::OnWidgetRebuilt();
}

TWeakObjectPtr<UOpenfortJSConnector> UOpenfortBrowserUserWidget::GetJSConnector() const
{
	if (!Browser)
	{
		OPENFORT_WARN("JSConnector requested before Browser was initialized");
		return nullptr;
	}

	return Browser->GetJSConnector();
}
