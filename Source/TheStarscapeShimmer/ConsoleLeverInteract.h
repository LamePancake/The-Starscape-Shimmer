// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "ProjectorInteract.h"
#include "ConsoleLeverInteract.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AConsoleLeverInteract : public AInteractableObject
{
public:
	GENERATED_BODY()

	// Sound for when the pull the lever
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		AAmbientSound* LeverSpeaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Console)
		AProjectorInteract* ProjectorInteract;

	// Locks into the puzzle if the film is currently in the puzzle zone.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Generator)
		void PullConsoleLever();

	void OnInteraction_Implementation(AFirstPersonCharacter* Character);
};
