// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Materials/Material.h>
#include "InteractableObject.h"
#include "TheatreScreen.h"
#include "FilmReelPickup.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
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
	double puzzleZoneStart = 0.0;
	double puzzleZoneEnd = 0.0;
	double puzzleDurationInMinutes = 4.0;
	double puzzleStartMin = 4;
	double puzzleStartMax = 16;
	bool inPuzzleZone = false;
	bool puzzleLocked = false;

	void Tick(float DeltaTime);

	AProjectorInteract()
	{
		HasPower = false;
		//PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"});
		CurrentFilmReel = NULL;

		// Get Reference to Movie Screen Material Parameter Collection
		static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> ScrnMatParamColl(TEXT("MaterialParameterCollection'/Game/ScreenMatParams'"));
		if (ScrnMatParamColl.Object != NULL)
		{
			ScreenMatParams = (UMaterialParameterCollection*)ScrnMatParamColl.Object;
		}
	}

	UPROPERTY() UMaterialParameterCollection* ScreenMatParams;

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

	void LockPuzzleZone();

	void UnlockPuzzleZone();

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
