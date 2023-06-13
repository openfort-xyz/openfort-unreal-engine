// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/OpenfortComponent.h"
#include "Misc/MessageDialog.h"


// Sets default values for this component's properties
UOpenfortComponent::UOpenfortComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenfortComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOpenfortComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UOpenfortComponent::GenerateKey()
{
	keyPair.Generate();
	FText dialog = FText::FromString(keyPair.GetPublicKeyHex().c_str());
	FMessageDialog::Open(EAppMsgType::Ok, dialog);
}
