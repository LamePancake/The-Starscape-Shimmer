// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "GeneratorInteract.h"

// Sets default values
AGeneratorInteract::AGeneratorInteract()
{
	isTurnedOn = false;
}

//Overrides the interation method
void AGeneratorInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	isTurnedOn = !isTurnedOn;
	UAudioComponent* SpeakerAudio = GeneratorSpeaker->GetAudioComponent();
	SpeakerAudio->Play();

	UAudioComponent* SpeakerAudio2 = GeneratorRunningSpeaker->GetAudioComponent();

	if (isTurnedOn)
	{
		SpeakerAudio2->Play();
	}
	else
	{
		SpeakerAudio2->Stop();
	}

	//TODO: Turn lights on here
}


