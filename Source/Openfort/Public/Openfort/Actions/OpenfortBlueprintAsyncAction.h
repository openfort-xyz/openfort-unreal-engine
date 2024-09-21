//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "OpenfortBlueprintAsyncAction.generated.h"
/**

 * base class for asynchronous actions
 */
UCLASS()
class OPENFORT_API UOpenfortBlueprintAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:
	UObject *WorldContextObject;

	// Get subsystem
	class UOpenfortSubsystem *GetSubsystem() const;
};
