// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "FilmReelPickup.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AFilmReelPickup : public APickup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FString FilmName = FString(TEXT("Death Gloop"));	
};
