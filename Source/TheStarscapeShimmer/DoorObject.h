// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "DoorObject.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ADoorObject : public AInteractableObject
{
	GENERATED_BODY()

	ADoorObject();
	
	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);

public:

	// Unlocks the door if the right key is passed to it.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Door)
	void UnlockDoor();

	// Tell us which door the key unlocks
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Door)
	float RequiredKey;

	// Tell us which door the key unlocks
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Door)
	bool IsLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* DoorSpeaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* DoorUnlockSpeaker;
};
