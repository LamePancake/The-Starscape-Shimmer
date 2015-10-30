// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "SafeInteract.h"

// Sets default values
ASafeInteract::ASafeInteract()
{
	Code = 1234;

	// The safe is locked.
	IsLocked = true;
}

//Overrides the interation method
void ASafeInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	UAudioComponent* SpeakerAudio = SafeSpeaker->GetAudioComponent();
	UAudioComponent* SpeakerAudio2 = SafeUnlockSpeaker->GetAudioComponent();

	if (!IsLocked)
	{
		SpeakerAudio2->Play();
		UnlockSafe();
		return;
	}

	//TODO: Add input code here to check if they endered the right combination
	if (IsLocked)
	{
		SpeakerAudio2->Play();
		IsLocked = false;
		UnlockSafe();
	}
	else
	{
		SpeakerAudio->Play();
	}
}


