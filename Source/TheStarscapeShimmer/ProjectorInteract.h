// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Materials/Material.h>
#include "InteractableObject.h"
#include "TheatreScreen.h"
#include "FilmReelPickup.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "ProjectorInteract.generated.h"


/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AProjectorInteract : public AInteractableObject
{
	GENERATED_BODY()

public:

	AProjectorInteract()
	{
		//PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"});
		CurrentFilmReel = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Screen)
		ATheatreScreen* TheatreScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Screen)
	AAmbientSound* TheatreSpeaker;


	void RunFilm(AFilmReelPickup*);

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reel)
	class AProjectorFilmReel* FilmReelBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reel)
	class AProjectorFilmReel* FilmReelFront;
	
	// Stores a reference to the film reel that is currently running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reel)
	class AFilmReelPickup* CurrentFilmReel;
};
