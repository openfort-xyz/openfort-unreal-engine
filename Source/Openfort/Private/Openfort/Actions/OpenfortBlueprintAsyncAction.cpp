// Fill out your copyright notice in the Description page of Project Settings.

#include "Openfort/Actions/OpenfortBlueprintAsyncAction.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Openfort/OpenfortSubsystem.h"

UOpenfortSubsystem *UOpenfortBlueprintAsyncAction::GetSubsystem() const
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		return nullptr;
	}
	return WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UOpenfortSubsystem>();
}
