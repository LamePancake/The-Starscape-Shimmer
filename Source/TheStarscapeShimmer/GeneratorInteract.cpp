// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "GeneratorInteract.h"

// Sets default values
AGeneratorInteract::AGeneratorInteract()
{

}

//Overrides the interation method
void AGeneratorInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);
	UAudioComponent* SpeakerAudio = GeneratorSpeaker->GetAudioComponent();
	SpeakerAudio->Play();
	//TODO: Play sound here
	//TODO: Turn lights on here
	UE_LOG(LogTemp, Warning, TEXT("Your message"));
}


