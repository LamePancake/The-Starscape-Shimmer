// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "KeyPickup.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AKeyPickup : public APickup
{
	GENERATED_BODY()

public:
	AKeyPickup();

	// Tell us which door the key unlocks
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Door)
	float WhichDoor;

	// Overrides the OnPickUp function, becauase blueprint native event
	void OnPickUp_Implementation();

};
