// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FilmReelPickup.h"

void AFilmReelPickup::BeginDestroy() {
	Super::BeginDestroy();
	if (!Film) return;
	Film->Pause();
	Film->Rewind();
}
