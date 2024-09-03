// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"

#include "OpenfortSaveGame.generated.h"

/**
 * Openfort sdk save game object
 */
UCLASS(BlueprintType, Blueprintable)
class OPENFORT_API UOpenfortSaveGame : public USaveGame
{
	GENERATED_UCLASS_BODY()

public:
	/** check if player logged in/connected with PKCE flow previously */
	UPROPERTY(VisibleAnywhere, Category = "Openfort")
	bool bWasConnectedViaPKCEFlow;
};
