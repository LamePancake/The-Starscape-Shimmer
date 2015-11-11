// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FilmReelPickup.h"
#include "ProjectorInteract.h"
#include "ProjectorFilmReel.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#ifdef WITH_EDITOR
#include "Runtime/Core/Public/Misc/OutputDevice.h"
#endif // WITH_EDITOR

//Overrides the interaction method
void AProjectorInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	// If the projector alread has a reel in it
	if (CurrentFilmReel != NULL)
	{
		// Cannot take it out if you are holding something else
		if (Character->HeldItem == nullptr)
		{
			StopFilm();
			// Give it to the player to hold.
			CurrentFilmReel->SetActorScale3D(FVector(3, 3, 3));
			Character->HeldItem = CurrentFilmReel;
			CurrentFilmReel = NULL;
			FilmReelBack->RunReel();
			FilmReelFront->RunReel();
		}
		return;
	}

	if (!HasPower)
		return;

	AFilmReelPickup* const Reel = Cast<AFilmReelPickup>(Character->HeldItem);
	CurrentFilmReel = Reel;
	
	// They're not holding a film reel, so do nothing :)
	if (!Reel) return;
	UE_LOG(LogTemp, Warning, TEXT("Reel Check"));
	
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

	FilmReelBack->RunReel();
	FilmReelFront->RunReel();

	// Drop the film reel
	//Character->HeldItem->OnDrop();
	Character->HeldItem = nullptr;

	// Hide the film reel in the projector, lol its a bit hacky
	CurrentFilmReel->SetActorLocation(this->GetActorLocation());
	CurrentFilmReel->SetActorScale3D(FVector(0, 0, 0));
}

///<summary>Stops the currently playing film and its audio, if any.</summary>
void AProjectorInteract::StopFilm()
{
	if (CurrentFilmReel == NULL) return;

	TArray<UStaticMeshComponent*> Comps;

	TheatreScreen->GetComponents(Comps);
	if (Comps.Num() > 0)
	{
		UStaticMeshComponent* Mesh = Comps[0];
		Mesh->SetMaterial(0, TheatreScreen->ScreenOffMaterial);
		CurrentFilmReel->Film->Pause();
	}

	UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
	SpeakerAudio->Stop();
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
	}

	UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
	SpeakerAudio->Stop();
	SpeakerAudio->SetSound(Reel->FilmSound);
	SpeakerAudio->Play();
}

void AProjectorInteract::Power_Implementation()
{
	HasPower = !HasPower;
}