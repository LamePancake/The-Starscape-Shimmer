// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "FilmReelPickup.generated.h"


/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AFilmReelPickup : public APickup
{
	GENERATED_BODY()
	
public:
	virtual ~AFilmReelPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMaterial* FilmMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMediaPlayer* Film;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Media)
		USoundWave* FilmSound;
};
