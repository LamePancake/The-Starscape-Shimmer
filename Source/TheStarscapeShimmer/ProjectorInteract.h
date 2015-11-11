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
		HasPower = false;
		//PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"});
		CurrentFilmReel = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Screen)
		ATheatreScreen* TheatreScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Screen)
		AAmbientSound* TheatreSpeaker;

	// Tells us if the projector has power or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projector)
		bool HasPower;

	// Turns the power on and off for the projector
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Projector)
		void Power();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Projector)
		void FastForward();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Projector)
		void PlayReverse();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Projector)
		void PlayNormal();

	void RunFilm(AFilmReelPickup*);

	void StopFilm();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);

	// Refence to the film reel on the back of the projector
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reel)
	class AProjectorFilmReel* FilmReelBack;

	// Reference to the Film reel on the front of the projector
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reel)
	class AProjectorFilmReel* FilmReelFront;

	// Stores a reference to the film reel that is currently running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reel)
	class AFilmReelPickup* CurrentFilmReel;
};
