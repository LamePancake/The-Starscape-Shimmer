// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FilmReelPickup.h"
#include "ProjectorInteract.h"

#ifdef WITH_EDITOR
#include "Runtime/Core/Public/Misc/OutputDevice.h"
#endif // WITH_EDITOR

//Overrides the interaction method
void AProjectorInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);
	AFilmReelPickup* const Reel = Cast<AFilmReelPickup>(Character->HeldItem);
	
	// They're not holding a film reel, so do nothing :)
	if (!Reel) return;
	
#ifdef WITH_EDITOR
	if (!Reel->Film || !Reel->FilmSound)
	{
		FMessageDialog::Debugf(FText::FromString("Trying to use film reel without film or sound. Add a film and its sound to the reel and try again."));
		return;
	}
#endif // WITH_EDITOR
	
	UE_LOG(LogTemp, Warning, TEXT("Attempting pick up %s"), *(Reel->Film->GetName()));
	// Do some logic here to get the film and play it from da screen
	this->RunFilm(Reel);
}

void AProjectorInteract::RunFilm(AFilmReelPickup* Reel)
{
	TArray<UStaticMeshComponent*> Comps;

	TheatreScreen->GetComponents(Comps);
	if (Comps.Num() > 0)
	{
		UStaticMeshComponent* Mesh = Comps[0];
		Mesh->SetMaterial(0, Reel->FilmMaterial);
		Reel->Film->Rewind();
		Reel->Film->Play();
	}

	UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
	SpeakerAudio->Stop();
	SpeakerAudio->SetSound(Reel->FilmSound);
	SpeakerAudio->Play();
}