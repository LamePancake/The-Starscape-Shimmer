// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FilmReelPickup.h"
#include "ProjectorInteract.h"

//Overrides the interation method
void AProjectorInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);
	AFilmReelPickup* const Reel = Cast<AFilmReelPickup>(Character->HeldItem);
	
	// They're not holding a film reel, so do nothing :)
	if (!Reel) return;
	
	// Do some logic here to get the film and play it from da screen
	UE_LOG(LogTemp, Warning, TEXT("Film's Name is %s"), Reel->FilmMaterial->GetName());
	this->RunFilm(Reel->FilmMaterial);
}
