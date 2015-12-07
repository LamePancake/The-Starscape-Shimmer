// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ConsoleLeverInteract.h"
#include "FirstPersonCharacter.h"

void AConsoleLeverInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	UAudioComponent* SpeakerAudio = LeverSpeaker->GetAudioComponent();
	
	if (ProjectorInteract->puzzleLocked)
	{
		ProjectorInteract->UnlockPuzzleZone();
		SpeakerAudio->Play();
		PullConsoleLever();
	}
	else
	{
		if (ProjectorInteract->inPuzzleZone) {
			SpeakerAudio->Play();
			ProjectorInteract->LockPuzzleZone();
			PullConsoleLever();
		}
	}
}


