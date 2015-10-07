// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "GeneratorInteract.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AGeneratorInteract : public AInteractableObject
{

public:
	GENERATED_BODY()

	AGeneratorInteract();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter* Character);
	
	// Sound for when the pull the lever
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* GeneratorSpeaker;

	// Sound for when the generator is running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* GeneratorRunningSpeaker;

	// True when the generator is on, false if it is turned off
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Object)
	bool isTurnedOn;
};
