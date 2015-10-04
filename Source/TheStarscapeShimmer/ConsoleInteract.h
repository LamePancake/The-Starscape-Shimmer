// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "ConsoleInteract.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AConsoleInteract : public AInteractableObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AConsoleInteract();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation();

	// Function to call when the pick up is interacted with
	UFUNCTION(BlueprintNativeEvent)
		void OnInteract();
};
