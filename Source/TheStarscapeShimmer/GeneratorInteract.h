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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* GeneratorSpeaker;
};
