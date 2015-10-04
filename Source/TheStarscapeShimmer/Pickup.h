// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "Pickup.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API APickup : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation();

	// Sets the location of the pick up when it has been picked up.
	UFUNCTION(BlueprintNativeEvent)
	void SetLocation(FVector loc, FRotator rot);

	UFUNCTION(BlueprintNativeEvent)
	void OnDrop();

	// Function to call when the pick up is collected
	UFUNCTION(BlueprintNativeEvent)
	void OnPickUp();
	
};
