// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FilmReelPickup.h"
#include "ProjectorInteract.h"
#include "ProjectorFilmReel.h"
#include <stdlib.h> 

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
		puzzleZoneStart = 0;
		puzzleZoneEnd = 0;
		inPuzzleZone = false;
		puzzleLocked = false;
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
		Reel->Film->Play();

		puzzleZoneStart = rand() % (int)(puzzleStartMax) + puzzleStartMin;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Start: %f"), puzzleZoneStart));
		puzzleZoneEnd = puzzleZoneStart + puzzleDurationInMinutes;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("End: %f"), puzzleZoneEnd));
	}

	UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
	SpeakerAudio->bStopWhenOwnerDestroyed = false;
	SpeakerAudio->Stop();
	SpeakerAudio->SetSound(Reel->FilmSound);
	SpeakerAudio->Play(0.0f);
}

void AProjectorInteract::Tick(float DeltaTime)
{
	if (CurrentFilmReel != NULL) {
		double minutesElapsed = CurrentFilmReel->Film->GetTime().GetTotalMinutes();
		if (!puzzleLocked && minutesElapsed >= puzzleZoneStart && minutesElapsed <= puzzleZoneEnd) {
			if (!inPuzzleZone)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("In Puzzle Zone")));

			inPuzzleZone = true;
			UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("PuzzleZone")), 1.0f);
		}
		
		if (puzzleLocked) {
			if (minutesElapsed > puzzleZoneEnd) {
				CurrentFilmReel->Film->Seek(FTimespan::FromMinutes(puzzleZoneStart));
			}
		} else if (inPuzzleZone) {
			if (minutesElapsed > puzzleZoneEnd) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Out of Puzzle Zone")));
				UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("PuzzleZone")), 0.0f);
				inPuzzleZone = false;
			}
		}
	}
}

void AProjectorInteract::LockPuzzleZone() 
{
	if (CurrentFilmReel->Film != NULL)
	{
		puzzleLocked = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Puzzle Zone Locked")));
		UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("PuzzleZoneLocked")), 1.0f);
	}
}

void AProjectorInteract::UnlockPuzzleZone()
{
	if (CurrentFilmReel->Film != NULL)
	{
		puzzleLocked = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Puzzle Zone Unlocked")));
		UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("PuzzleZoneLocked")), 0.0f);
	}
}

void AProjectorInteract::Power_Implementation()
{
	HasPower = !HasPower;
}

void AProjectorInteract::ScalePlayRate(float scale) {
	
	int playRate = scale * maxPlayRate;

	if (playRate == 0) playRate = 1;
	
	if (CurrentFilmReel->Film != NULL && playRate != currentPlayRate) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Changed Rate %d"), playRate));

		CurrentFilmReel->Film->SetRate(playRate);

		/*if (playRate == 1) { // Rate normal again so play audio again
			TheatreSpeaker->Play(CurrentFilmReel->Film->GetTime().GetTotalSeconds());
		}*/
	}
}

void AProjectorInteract::FastForward_Implementation()
{
	if (HasPower) {
		CurrentFilmReel->Film->SetRate(16);
		UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
		SpeakerAudio->Stop();
	}
}

void AProjectorInteract::PlayReverse_Implementation()
{
	if (HasPower) {
		CurrentFilmReel->Film->SetRate(-16);
		UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
		SpeakerAudio->Stop();
	}
}

void AProjectorInteract::PlayNormal_Implementation()
{
	if (HasPower) {
		UAudioComponent* SpeakerAudio = TheatreSpeaker->GetAudioComponent();
		CurrentFilmReel->Film->SetRate(1);
		SpeakerAudio->Play(CurrentFilmReel->Film->GetTime().GetTotalSeconds());

		//FTimespan currentTime = CurrentFilmReel->Film->GetTime();
		//SpeakerAudio->Stop();
		//UE_LOG(LogTemp, Warning, TEXT("timespan Seconds: %f"), currentTime.GetSeconds());
		//SpeakerAudio->Play(currentTime.GetTotalSeconds());
	}
}