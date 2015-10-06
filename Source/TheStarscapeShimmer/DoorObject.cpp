// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "DoorObject.h"
#include "KeyPickup.h"

// Sets default values
ADoorObject::ADoorObject()
{
	// Set the key to none for now, but it will probably stay
	RequiredKey = -1.0f;

	// The door is locked.
	IsLocked = true;
}

//Overrides the interation method
void ADoorObject::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);
}

// Checks the door with the key to see if the key can open the door
void ADoorObject::UnlockDoor(AKeyPickup* k)
{
	if (IsLocked && RequiredKey == k->WhichDoor)
	{
		//Unlock door here
		Destroy();
	}
}
