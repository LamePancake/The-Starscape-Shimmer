// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "DoorObject.h"
#include "KeyPickup.h"

// Sets default values
ADoorObject::ADoorObject()
{
	// Set the key to none for now, but it will probably stay
	RequiredKey = -1.0f;
}

//Overrides the interation method
void ADoorObject::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	if (!IsLocked)
	{
		UnlockDoor();
		return;
	}

	AKeyPickup* Key = Cast<AKeyPickup>(Character->HeldItem);
	UAudioComponent* SpeakerAudio = DoorSpeaker->GetAudioComponent();
	UAudioComponent* SpeakerAudio2 = DoorUnlockSpeaker->GetAudioComponent();
	
	if (Key)
	{
		SpeakerAudio2->Play();
		if (IsLocked && RequiredKey == Key->WhichDoor)
		{
			UnlockDoor();
			IsLocked = false;
		}
	}
	else
	{
		SpeakerAudio->Play();
	}
}
